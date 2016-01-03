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
};

struct Dimensions
{
    int width;
    int height;
};

enum class Rotation90 { DEG_0, DEG_90, DEG_180, DEG_270 };

enum DatatypeFlags
{
    EMPTY = 0x0,
    RGB_CAMERA_FLAG = 0x1,
    DEPTH_CAMERA_FLAG = 0x2
};

}

