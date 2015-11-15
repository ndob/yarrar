#include "WebcamDataProvider.h"

namespace yarrar
{

WebcamDataProvider::WebcamDataProvider():
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

}


