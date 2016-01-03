#include "tracker/marker/MarkerTracker.h"
#include "tracker/marker/YarrarMarkerParser.h"
#include "Util.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <sstream>
#include <vector>

#if _MSC_VER
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API
#endif

namespace {

const int PREFERRED_TRACKING_RESOLUTION_WIDTH = 320;
std::string returnStringBuffer;

void storePoseToReturnBuffer(const yarrar::Pose& pose)
{
    std::ostringstream ss;
    ss << "{ \"valid\": ";
    ss << "true";

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

        cv::Size trackingRes = yarrar::getScaledDownResolution(width, height, PREFERRED_TRACKING_RESOLUTION_WIDTH);
        yarrar::MarkerTracker<yarrar::YarrarMarkerParser> detector(trackingRes);
        std::vector<yarrar::Pose> poses;
        detector.getPoses(flipped, poses);

        if(poses.size() > 0) storePoseToReturnBuffer(poses[0]);
        return returnStringBuffer.c_str();
    }
}
