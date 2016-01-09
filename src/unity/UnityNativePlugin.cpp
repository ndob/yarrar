#include "tracker/marker/MarkerTracker.h"
#include "tracker/marker/YarrarMarkerParser.h"
#include "Util.h"

#include <opencv2/opencv.hpp>
#include <json11.hpp>

#include <string>
#include <sstream>
#include <vector>

#if _MSC_VER
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API
#endif

namespace
{

std::string returnStringBuffer;

void storePoseToReturnBuffer(const yarrar::Pose& pose)
{
    std::vector<double> rotationMatrix;
    rotationMatrix.reserve(9);
    std::vector<double> translationMatrix;
    translationMatrix.reserve(3);
    std::vector<double> cameraMatrix;
    cameraMatrix.reserve(9);

    cv::Mat rotationMtx;
    cv::Rodrigues(pose.rotation, rotationMatrix);
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            rotationMatrix.push_back(rotationMtx.at<double>(i, j));
        }
    }

    for(int i = 0; i < 3; ++i)
    {
        translationMatrix.push_back(pose.translation.at<double>(i));
    }

    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            cameraMatrix.push_back(pose.camera.at<float>(i, j));
        }
    }

    json11::Json poseJson = json11::Json::object{
        { "valid", true },
        { "rotation", rotationMatrix },
        { "translation", translationMatrix },
        { "camera", cameraMatrix }
    };
    poseJson.dump(returnStringBuffer);
}
}

extern "C" {

const EXPORT_API char* getPose(void* pixelBuffer, int width, int height)
{
    cv::Mat argb(height, width, CV_8UC4, pixelBuffer);
    cv::Mat bgr, flipped;
    cv::cvtColor(argb, bgr, CV_RGBA2BGR);
    cv::flip(bgr, flipped, 0);

    // TODO: This should be initialized only when configuration changes.
    json11::Json markerTrackerConf = json11::Json::object{
        { "parser", "yarrar_parser" },
        { "tracking_resolution_width", 320 }
    };

    yarrar::MarkerTracker tracker(width, height, markerTrackerConf);
    std::vector<yarrar::Pose> poses;
    tracker.getPoses(flipped, poses);

    if(poses.size() > 0) storePoseToReturnBuffer(poses[0]);
    return returnStringBuffer.c_str();
}
}
