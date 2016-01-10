#include "AndroidCameraProvider.h"

namespace yarrar
{

int AndroidCameraProvider::s_width = 0;
int AndroidCameraProvider::s_height = 0;
LockableData<Datapoint> AndroidCameraProvider::s_dp({});

AndroidCameraProvider::AndroidCameraProvider(const json11::Json& config)
    : DataProvider(config)
{
}

const LockableData<Datapoint>& AndroidCameraProvider::getData()
{
    return s_dp;
}

Dimensions AndroidCameraProvider::getDimensions()
{
    return {
        s_width, s_height
    };
}

DatatypeFlags AndroidCameraProvider::provides()
{
    return RGB_CAMERA_FLAG;
}

void AndroidCameraProvider::injectCameraFrame(const cv::Mat& rgb)
{
    auto handle = s_dp.lockReadWrite();
    handle.set({ TimestampClock::now(),
        rgb });
}

void AndroidCameraProvider::injectCameraSize(const int width, const int height)
{
    // FIXME: Switch dimensions as a workaround
    // for getting portrait orientation to work correctly.
    s_width = height;
    s_height = width;
}
}
