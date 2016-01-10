#include "tracker/marker/MarkerTracker.h"
#include "tracker/marker/YarrarMarkerParser.h"
#include "Util.h"

#include <opencv2/opencv.hpp>
#include <json11.hpp>

#include <string>
#include <vector>

#if _MSC_VER
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API
#endif

namespace
{

yarrar::MarkerTracker* s_tracker;
std::string s_returnStringBuffer;

void addPose(json11::Json::array& toJson, const yarrar::Pose& pose)
{
    std::vector<double> rotationMatrix;
    rotationMatrix.reserve(9);
    std::vector<double> translationMatrix;
    translationMatrix.reserve(3);
    std::vector<double> cameraMatrix;
    cameraMatrix.reserve(9);

    cv::Mat rotationMtx;
    cv::Rodrigues(pose.rotation, rotationMtx);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            rotationMatrix.push_back(rotationMtx.at<double>(i, j));
        }
    }
    
    for (int i = 0; i < 3; ++i)
    {
        translationMatrix.push_back(pose.translation.at<double>(i));
    }

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            cameraMatrix.push_back(pose.camera.at<float>(i, j));
        }
    }
    
    json11::Json poseJson = json11::Json::object{
        { "rotation", rotationMatrix },
        { "translation", translationMatrix },
        { "camera", cameraMatrix },
        { "coordinateSystemId", pose.coordinateSystemId }
    };

    toJson.push_back(poseJson);
}

void storePosesToReturnBuffer(const std::vector<yarrar::Pose>& poses)
{
    // Add all poses to a JSON-array.
    json11::Json::array poseArray;
    for (const auto& pose : poses)
    {
        addPose(poseArray, pose);
    }

    json11::Json ret = json11::Json::object{
        { "poses", poseArray }
    };

    ret.dump(s_returnStringBuffer);
}
}

extern "C" {

void EXPORT_API initYarrar(int width, int height /*,TODO: const char* jsonString */)
{
    json11::Json markerTrackerConf = json11::Json::object{
        { "parser", "yarrar_parser" },
        { "tracking_resolution_width", 320 }
    };

    s_tracker = new yarrar::MarkerTracker(width, height, markerTrackerConf);
}

void EXPORT_API deinitYarrar()
{
    delete s_tracker;
}

const EXPORT_API char* getPose(void* pixelBuffer, int width, int height)
{
    cv::Mat argb(height, width, CV_8UC4, pixelBuffer);
    cv::Mat bgr, flipped;
    cv::cvtColor(argb, bgr, CV_RGBA2BGR);
    cv::flip(bgr, flipped, 0);

    yarrar::Datapoint datapoint{
        yarrar::TimestampClock::now(),
        flipped
    };

    std::vector<yarrar::Pose> poses;
    s_tracker->getPoses(datapoint, poses);
    
    s_returnStringBuffer.clear();
    if (poses.size() > 0)
    {
        storePosesToReturnBuffer(poses);
    }

    return s_returnStringBuffer.c_str();
}
}
