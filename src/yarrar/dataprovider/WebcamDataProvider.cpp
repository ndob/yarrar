#include "WebcamDataProvider.h"

namespace yarrar
{

WebcamDataProvider::WebcamDataProvider(const json11::Json& config):
    DataProvider(config),
    m_videoCapture(0)
{
    if(!m_videoCapture.isOpened())
    {
        throw std::runtime_error("cant open video");
    }
}


cv::Mat WebcamDataProvider::getData()
{
    cv::Mat ret;
    m_videoCapture >> ret;
    return ret;
}

Dimensions WebcamDataProvider::getDimensions()
{
    const auto& data = getData();
    return {
        data.cols,
        data.rows
    };
}

DatatypeFlags WebcamDataProvider::provides()
{
    return RGB_CAMERA_FLAG;
}

}


