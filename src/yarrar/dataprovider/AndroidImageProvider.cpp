#include "AndroidImageProvider.h"

namespace yarrar
{

int AndroidImageProvider::s_width = 0;
int AndroidImageProvider::s_height = 0;
LockableData<Datapoint> AndroidImageProvider::s_dp({});

AndroidImageProvider::AndroidImageProvider(const json11::Json& config)
    : DataProvider(config)
{
}

const LockableData<Datapoint>& AndroidImageProvider::getData()
{
    return s_dp;
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
    auto handle = s_dp.lockReadWrite();
    handle.set({ std::chrono::high_resolution_clock::now(),
        rgb });
}

void AndroidImageProvider::injectCameraSize(const int width, const int height)
{
    // FIXME: Switch dimensions as a workaround
    // for getting portrait orientation to work correctly.
    s_width = height;
    s_height = width;
}
}
