#pragma once

#include <opencv2/opencv.hpp>
#include <chrono>

namespace yarrar
{
using TimestampClock = std::chrono::steady_clock;
using Timestamp = std::chrono::time_point<TimestampClock>;

struct Datapoint
{
    Timestamp created;
    cv::Mat data;
};

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

enum class Rotation90
{
    DEG_0,
    DEG_90,
    DEG_180,
    DEG_270
};

enum DatatypeFlags
{
    EMPTY = 0x0,
    RGB_CAMERA_FLAG = 0x1,
    DEPTH_CAMERA_FLAG = 0x2,
    ORIENTATION_SENSOR_FLAG = 0x4
};
}
