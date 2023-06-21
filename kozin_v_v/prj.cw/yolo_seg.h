#pragma once
#include<iostream>
#include<cassert>
#include<opencv2/opencv.hpp>
#include "yolov5_seg_utils.h"

class YoloSeg {
public:
	YoloSeg() {
	}
	~YoloSeg() {}
	/** \brief Read onnx-model
	* \param[out] read onnx file into cv::dnn::Net
	* \param[in] modelPath:onnx-model path
	* \param[in] isCuda:if true and opencv built with CUDA(cmake),use OpenCV-GPU,else run it on cpu.
	*/
	bool ReadModel(cv::dnn::Net& net, std::string& netPath, bool isCuda);
	/** \brief  detect.
	* \param[in] srcImg:a 3-channels image.
	* \param[out] output:detection results of input image.
	*/
	bool Detect(cv::Mat& srcImg, cv::dnn::Net& net, std::vector<OutputSeg>& output);

#if(defined YOLO_P6 && YOLO_P6==true)

	const int _netWidth = 1280;
	const int _netHeight = 1280;
	const int _segWidth = 320;  //_segWidth=_netWidth/mask_ratio
	const int _segHeight = 320;
	const int _segChannels = 32;
#else

	const int _netWidth = 640;
	const int _netHeight = 640;
	const int _segWidth = 160;    //_segWidth=_netWidth/mask_ratio
	const int _segHeight = 160;
	const int _segChannels = 32;

#endif // YOLO_P6

	float _classThreshold = 0.25f;
	float _nmsThreshold = 0.45f;
	float _maskThreshold = 0.5f;

public:
//	std::vector<std::string> _className = { "p", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
//		"fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
//		"elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
//		"skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
//		"tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
//		"sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
//		"potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
//		"microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
//		"hair drier", "toothbrush" };
    std::vector<std::string> _className = {"car"};
};
