#pragma once

#include "Types.h"

#include <opencv2/opencv.hpp>

namespace yarrar
{

struct MarkerValue
{
    bool valid;
    int id;
    Rotation90 zRotation;
};

class MarkerParser
{
public:
    virtual ~MarkerParser(){};
    virtual MarkerValue getData(const cv::Mat& image) = 0;
};
}
