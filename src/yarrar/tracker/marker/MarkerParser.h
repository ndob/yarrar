#pragma once

#include <opencv2/opencv.hpp>

namespace yarrar {

enum class MarkerRotation { DEG_0, DEG_90, DEG_180, DEG_270 };

struct MarkerValue
{
    int id;
    MarkerRotation zRotation;
};

class MarkerParser
{
public:
    virtual ~MarkerParser() {};
    virtual MarkerValue getData(const cv::Mat& image) = 0;
};

}
