#include"yolo_seg.h"
#include <opencv2/core/utils/logger.hpp>

using namespace std;
using namespace cv;
using namespace cv::dnn;




bool YoloSeg::ReadModel(Net& net, string& netPath, bool isCuda = false) {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);
	try {
		net = readNet(netPath);
#if CV_VERSION_MAJOR==4 &&CV_VERSION_MINOR==7&&CV_VERSION_REVISION==0
		net.enableWinograd(false);
		//net.enableWinograd(true);		//If your CPU supports AVX2, you can set it true to speed up
#endif
	}
	catch (const std::exception&) {
		return false;
	}
	if (isCuda) {
		//cuda
		net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
		net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA); //or DNN_TARGET_CUDA_FP16
	}
	else {
		//cpu
		cout << "Inference device: CPU" << endl;
		net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
		net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
	}
	return true;
}


bool YoloSeg::Detect(Mat& srcImg, Net& net, vector<OutputSeg>& output) {
	Mat blob;
	output.clear();
	int col = srcImg.cols;
	int row = srcImg.rows;
	Mat netInputImg;
	Vec4d params;
	LetterBox(srcImg, netInputImg, params, cv::Size(_netWidth, _netHeight));
	blobFromImage(netInputImg, blob, 1 / 255.0, cv::Size(_netWidth, _netHeight), cv::Scalar(0, 0, 0), true, false);

	net.setInput(blob);
	std::vector<cv::Mat> net_output_img;
	vector<string> output_layer_names{ "output0","output1" };
	net.forward(net_output_img, output_layer_names);

	std::vector<int> class_ids;
	std::vector<float> confidences;
	std::vector<cv::Rect> boxes;
	std::vector<vector<float>> picked_proposals;
	int net_width = _className.size() + 5 + _segChannels;
	int out0_width= net_output_img[0].size[2];

	int net_height = net_output_img[0].size[1];
	float* pdata = (float*)net_output_img[0].data;
	for (int r = 0; r < net_height; r++) {    //lines
		float box_score = pdata[4];
		if (box_score >= _classThreshold) {
			cv::Mat scores(1, _className.size(), CV_32FC1, pdata + 5);
			Point classIdPoint;
			double max_class_socre;
			minMaxLoc(scores, 0, &max_class_socre, 0, &classIdPoint);
			max_class_socre = (float)max_class_socre;
			if (max_class_socre >= _classThreshold) {

				vector<float> temp_proto(pdata + 5 + _className.size(), pdata + net_width);
				picked_proposals.push_back(temp_proto);
				//rect [x,y,w,h]
				float x = (pdata[0] - params[2]) / params[0];  //x
				float y = (pdata[1] - params[3]) / params[1];  //y
				float w = pdata[2] / params[0];  //w
				float h = pdata[3] / params[1];  //h
				int left = MAX(int(x - 0.5 * w + 0.5), 0);
				int top = MAX(int(y - 0.5 * h + 0.5), 0);
				class_ids.push_back(classIdPoint.x);
				confidences.push_back(max_class_socre * box_score);
				boxes.push_back(Rect(left, top, int(w + 0.5), int(h + 0.5)));
			}
		}
		pdata += net_width;

	}

	//NMS
	vector<int> nms_result;
	cv::dnn::NMSBoxes(boxes, confidences, _classThreshold, _nmsThreshold, nms_result);
	std::vector<vector<float>> temp_mask_proposals;
	Rect holeImgRect(0, 0, srcImg.cols, srcImg.rows);
	for (int i = 0; i < nms_result.size(); ++i) {

		int idx = nms_result[i];
		OutputSeg result;
		result.id = class_ids[idx];
		result.confidence = confidences[idx];
		result.box = boxes[idx] & holeImgRect;
		temp_mask_proposals.push_back(picked_proposals[idx]);
		output.push_back(result);
	}

	MaskParams mask_params;
	mask_params.params = params;
	mask_params.srcImgShape = srcImg.size();
	for (int i = 0; i < temp_mask_proposals.size(); ++i) {
		GetMask2(Mat(temp_mask_proposals[i]).t(), net_output_img[1], output[i], mask_params);
	}




	if (output.size())
		return true;
	else
		return false;
}

