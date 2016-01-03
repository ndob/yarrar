#include "AndroidImageProvider.h"

namespace yarrar {

cv::Mat AndroidImageProvider::s_image;
std::mutex AndroidImageProvider::s_imageMutex;

AndroidImageProvider::AndroidImageProvider(const json11::Json &config):
    DataProvider(config)
{
}

cv::Mat AndroidImageProvider::getData()
{
    std::lock_guard <std::mutex> m_(s_imageMutex);
    return s_image;
}

Dimensions AndroidImageProvider::getDimensions()
{
    // TODO: Remove hard coding.
    return {
        960, 720
    };
}

DatatypeFlags AndroidImageProvider::provides()
{
    return RGB_CAMERA_FLAG;
}

void AndroidImageProvider::injectCameraFrame(const cv::Mat& rgb)
{
    std::lock_guard<std::mutex> m_(s_imageMutex);
    s_image = rgb;
}

}
