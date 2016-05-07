#include "IOSCameraProvider.h"

namespace yarrar
{

int IOSCameraProvider::s_width = 0;
int IOSCameraProvider::s_height = 0;
LockableData<Datapoint> IOSCameraProvider::s_dp({});

IOSCameraProvider::IOSCameraProvider(const json11::Json& config)
    : DataProvider(config)
{
}

const LockableData<Datapoint>& IOSCameraProvider::getData()
{
    return s_dp;
}

Dimensions IOSCameraProvider::getDimensions()
{
    return {
        s_width, s_height
    };
}

DatatypeFlags IOSCameraProvider::provides()
{
    return RGB_CAMERA_FLAG;
}

void IOSCameraProvider::injectCameraFrame(const cv::Mat& rgb)
{
    auto handle = s_dp.lockReadWrite();
    handle.set({ TimestampClock::now(),
        rgb });
}

void IOSCameraProvider::injectCameraSize(const int width, const int height)
{
    s_width = width;
    s_height = height;
}
}
