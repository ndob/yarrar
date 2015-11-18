#pragma once
#include <opencv2/opencv.hpp>

namespace yarrar
{

struct Pose
{
    cv::Mat rotation;
    cv::Mat translation;
    cv::Mat camera;
    int coordinateSystemId;
    bool valid;
};

}
