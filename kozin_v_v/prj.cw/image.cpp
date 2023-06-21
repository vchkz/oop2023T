#include "image.h"



Image::Image() = default;

Image::~Image() {
    if (_data != nullptr) {
        delete[] _data;
        _data = nullptr;
    }
}


Layer *Image::getMask() {
    return _layers[1];
}

int Image::UndoStackSize() {
    return _undoStack.size();
}

void Image::addLayer() {
    byte pixel[4] = {0, 0, 0, 0};
    auto layer = Layer::defaultLayer(size, pixel);
    layer->name = std::string("Layer ");

    _layers.push_back(layer);

}

void Image::fromFile(const char *filename) {
    _layers.clear();

    auto img = Layer::fromFile(filename);
    img->name = std::string("input image");
    img->bpp = 4;
    size[0] = img->size[0];
    size[1] = img->size[1];
    _layers.push_back(img);
    addLayer();
    addLayer();

    isOpen = true;

}

void Image::fromMAT(const cv::Mat &mask, const std::string &name) {
    auto layer = _layers[1];
    layer->name = name;
    layer->size[0] = mask.cols;
    layer->size[1] = mask.rows;


    layer->bpp = 4;


    cv::Mat outputImage(mask.rows, mask.cols, CV_8UC4);

    for (int y = 0; y < mask.rows; y++) {
        for (int x = 0; x < mask.cols; x++) {
            cv::Vec3b color = mask.at<cv::Vec3b>(y, x);
            if (color[0] == 255 && color[1] == 255 && color[2] == 255) {
                cv::Vec4b newColor(255, 0, 0, 255);
                outputImage.at<cv::Vec4b>(y, x) = newColor;
            } else {
                cv::Vec4b newColor(0, 0, 0, 0);
                outputImage.at<cv::Vec4b>(y, x) = newColor;
            }
        }
    }
    int dataSize = layer->size[0] * layer->size[1] * layer->bpp;

    layer->data = new unsigned char[dataSize];

    if (outputImage.isContinuous()) {
        memcpy(layer->data, outputImage.data, dataSize);
    } else {
        unsigned char *ptr = layer->data;
        for (int i = 0; i < outputImage.rows; i++) {
            memcpy(ptr, outputImage.ptr(i), outputImage.cols * layer->bpp);
            ptr += outputImage.cols * layer->bpp;
        }
    }

    _layers.push_back(layer);

}


void Image::deleteCursor() {
    byte color[4] = {0, 0, 0, 0};
    for (int x = -40; x <= 40; x++) {
        for (int y = -40; y <= 40; y++) {
            if ((_lastCursorPosition[0] + x >= 0) && (_lastCursorPosition[0] + x < size[0]) &&
                (_lastCursorPosition[1] + y >= 0) && (_lastCursorPosition[1] + y < size[1])) {
                _layers[2]->setPixel(_lastCursorPosition[0] + x, _lastCursorPosition[1] + y, color);
            }
        }
    }
    setDirty(true);
}

void Image::drawCursor(int X, int Y) {
    int radius = _cursorSize;
    deleteCursor();
    byte pixel[4] = {255, 255, 255, 255};

    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            double distance = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
            if ((X + x >= 0) && (X + x < size[0]) && (Y + y >= 0) && (Y + y < size[1]) &&
                (std::abs(distance - radius) <= 0.5 || std::abs(distance - (radius - 1)) <= 0.5)) {
                _layers[2]->setPixel(X + x, Y + y, pixel);
            }
        }
    }
    setDirty(true);
}

void Image::resizeCursor(int x) {
    if (_cursorSize > 1 && x < 0) { _cursorSize--; }
    if (_cursorSize < 40 && x > 0) { _cursorSize++; }
}

void Image::mouseMove(int x, int y) {
    drawCursor(x, y);

    auto from = glm::vec2(_lastCursorPosition[0], _lastCursorPosition[1]);
    auto to = glm::vec2(x, y);


    _lastCursorPosition[0] = x;
    _lastCursorPosition[1] = y;

    for (int i = 0; i < 2; i++)
        if (_lastCursorPosition[i] < -40 || _lastCursorPosition[i] >= size[i] + 40)
            return;
    if (_isPainting)
        paint(_layers[1], brushColor, from, to, static_cast<float>(_cursorSize));
    else if (_isErasing) paint(_layers[1], eraseColor, from, to, static_cast<float>(_cursorSize));


}

void Image::rightMouseButton() {
    if (_lastCursorPosition[0] < -40 || _lastCursorPosition[0] >= size[0] + 40 || _lastCursorPosition[1] < -40 ||
        _lastCursorPosition[1] >= size[1] + 40) {
        return;
    }
    toCtrlZStack();


    _isErasing = true;

    auto to = glm::vec2(static_cast<float>(_lastCursorPosition[0]), static_cast<float>(_lastCursorPosition[1]));
    paint(_layers[1], eraseColor, to, to, static_cast<float>(_cursorSize));
}

void Image::leftMouseButton() {
    if (_lastCursorPosition[0] < -40 || _lastCursorPosition[0] >= size[0] + 40 || _lastCursorPosition[1] < -40 ||
        _lastCursorPosition[1] >= size[1] + 40) {
        return;
    }
    toCtrlZStack();

    maskIsCreated = true;

    _isPainting = true;
    auto to = glm::vec2(_lastCursorPosition[0], _lastCursorPosition[1]);
    paint(_layers[1], brushColor, to, to, static_cast<float>(_cursorSize));


}

void Image::mouseButtonUp() {
    _isPainting = false;
    _isErasing = false;
    setDirtyForSaving(true);
}


void Image::paint(Layer *layer, float color[], const glm::vec2 &from, const glm::vec2 &to, float brushWidth) {
    if (!_isPainting && !_isErasing) {
        return;
    }


    auto dir = to - from;
    auto ndir = glm::normalize(dir);

    glm::vec2 pixelPos = from;
    byte pixel[4];
    for (int b = 0; b < layer->bpp; b++) {
        pixel[b] = static_cast<byte>(color[b] * 255.0f);
    }


    for (int i = 0; i < glm::length(dir) + 1; i++) {
        for (int x = static_cast<int>(-brushWidth); x <= static_cast<int>(brushWidth); x++) {
            for (int y = static_cast<int>(-brushWidth); y <= static_cast<int>(brushWidth); y++) {
                if ((glm::length(glm::vec2(x, y)) < brushWidth) &&
                    (pixelPos.x + x >= 0) && (pixelPos.x + x < layer->size[0]) && (pixelPos.y + y >= 0) &&
                    (pixelPos.y + y < layer->size[1])) {
                    layer->setPixel(static_cast<int>(pixelPos.x) + x, static_cast<int>(pixelPos.y) + y, pixel);
                }
            }
        }

        pixelPos += ndir;
    }
    setDirty(true);


}

void Image::Undo() {
    if (!_undoStack.empty()) {
        _layers.at(1) = _undoStack.back();
        _undoStack.pop_back();
        setDirtyForSaving(true);
        setDirty(true);
    }
}

void Image::toCtrlZStack() {
    auto layer = new Layer();
    layer->name = _layers[2]->name;
    layer->size[0] = _layers[2]->size[0];
    layer->size[1] = _layers[2]->size[1];
    layer->bpp = 4;
    int dataSize = layer->size[0] * layer->size[1] * 4;
    layer->data = new unsigned char[dataSize];
    memcpy(layer->data, _layers[1]->data, dataSize);
    _undoStack.push_back(layer);
}


void Image::texImage() {
    auto dataSize = size[0] * size[1] * 4;
    if (_data == nullptr) _data = new unsigned char[dataSize];
    memset(_data, 0, dataSize);

    for (int i = 0; i < 3; ++i) {
        auto layer = _layers[i];
        if (!layer->isVisible()) continue;
        for (int y = 0; y < size[1]; ++y) {
            for (int x = 0; x < size[0]; ++x) {
                auto p = (x * size[1] + y);
                float tempAlpha = layer->data[p * 4 + 3] / 255.0;
                if (i == 1)tempAlpha *= alpha;
                for (int b = 0; b < 3; b++) {
                    _data[p * 4 + b] = static_cast<int>(layer->data[p * 4 + b] * tempAlpha +
                                                        _data[p * 4 + b] * (1 - tempAlpha));
                }
                _data[p * 4 + 3] = std::min(255, _data[p * 4 + 3] + layer->data[p * 4 + 3]);
            }
        }
    }

    if (_glindex == 0) glGenTextures(1, &(_glindex));
    glBindTexture(GL_TEXTURE_2D, _glindex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GLint format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, size[0], size[1], 0, format, GL_UNSIGNED_BYTE, _data);

    setDirty(false);
}