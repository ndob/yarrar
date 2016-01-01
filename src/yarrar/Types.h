#pragma once

#include <opencv2/opencv.hpp>

namespace yarrar {

struct Vector3
{
    float x;
    float y;
    float z;
};

struct Pose
{
    cv::Mat rotation;
    cv::Mat translation;
    cv::Mat camera;
    int coordinateSystemId;
    bool valid;
};

struct Dimensions
{
    int width;
    int height;
};

enum class Rotation90 { DEG_0, DEG_90, DEG_180, DEG_270 };

}

