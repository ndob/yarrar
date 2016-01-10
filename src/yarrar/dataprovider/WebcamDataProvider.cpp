#include "WebcamDataProvider.h"

namespace yarrar
{

WebcamDataProvider::WebcamDataProvider(const json11::Json& config)
    : DataProvider(config)
    , m_videoCapture(0)
    , m_dp({})
{
    if(!m_videoCapture.isOpened())
    {
        throw std::runtime_error("cant open video");
    }
}


const LockableData<Datapoint>& WebcamDataProvider::getData()
{
    cv::Mat ret;
    m_videoCapture >> ret;

    auto handle = m_dp.lockReadWrite();
    handle.set({ std::chrono::high_resolution_clock::now(),
        ret });

    return m_dp;
}

Dimensions WebcamDataProvider::getDimensions()
{
    return {
        static_cast<int>(m_videoCapture.get(CV_CAP_PROP_FRAME_WIDTH)),
        static_cast<int>(m_videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT))
    };
}

DatatypeFlags WebcamDataProvider::provides()
{
    return RGB_CAMERA_FLAG;
}
}
