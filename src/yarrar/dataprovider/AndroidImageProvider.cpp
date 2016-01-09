#include "AndroidImageProvider.h"

namespace yarrar
{

int AndroidImageProvider::s_width = 0;
int AndroidImageProvider::s_height = 0;
cv::Mat AndroidImageProvider::s_image;
std::mutex AndroidImageProvider::s_imageMutex;

AndroidImageProvider::AndroidImageProvider(const json11::Json& config)
    : DataProvider(config)
{
}

cv::Mat AndroidImageProvider::getData()
{
    std::lock_guard<std::mutex> m_(s_imageMutex);
    return s_image;
}

Dimensions AndroidImageProvider::getDimensions()
{
    return {
        s_width, s_height
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

void AndroidImageProvider::injectCameraSize(const int width, const int height)
{
    // FIXME: Switch dimensions as a workaround
    // for getting portrait orientation to work correctly.
    s_width = height;
    s_height = width;
}
}
