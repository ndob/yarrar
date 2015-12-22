#include "detector/MarkerDetector.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <sstream>

#if _MSC_VER
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API
#endif

namespace {

std::string returnStringBuffer;

void storePoseToReturnBuffer(const yarrar::Pose& pose)
{
    std::ostringstream ss;
    ss << "{ \"valid\": ";
    ss << pose.valid ? "true" : "false";

    if (pose.valid)
    {
        cv::Mat rotationMatrix;
        cv::Rodrigues(pose.rotation, rotationMatrix);
        
        ss << ",";
        ss << "\"rotation\": [";
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                ss << rotationMatrix.at<double>(i, j);
                if (j != 2)
                {
                    ss << ",";
                }
            }
            if (i != 2)
            {
                ss << ",";
            }
        }
        ss << "]";

        ss << ",";
        ss << "\"translation\": [";
        for (int i = 0; i < 3; ++i)
        {
            ss << pose.translation.at<double>(i);
            if (i != 2)
            {
                ss << ",";
            }
        }
        ss << "]";

        ss << ",";
        ss << "\"camera\": [";
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                ss << pose.camera.at<float>(i, j);
                if (j != 2)
                {
                    ss << ",";
                }
            }
            if (i != 2)
            {
                ss << ",";
            }
        }
        ss << "]";

    }

    ss << "}";

    returnStringBuffer = ss.str();
}

}

extern "C"
{
    const EXPORT_API char* getPose(void* pixelBuffer, int width, int height)
    {
        cv::Mat argb(height, width, CV_8UC4, pixelBuffer);
        cv::Mat bgr, flipped;
        cv::cvtColor(argb, bgr, CV_RGBA2BGR);
        cv::flip(bgr, flipped, 0);

        yarrar::MarkerDetector detector(width, height);
        auto pose = detector.getPose(flipped);

        storePoseToReturnBuffer(pose);
        return returnStringBuffer.c_str();
    }
}
