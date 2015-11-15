#pragma once

#include "Pipeline.h"

#include <opencv2/videoio.hpp>

namespace yarrar
{

class WebcamDataProvider: public DataProvider
{
public:
    WebcamDataProvider();
    cv::Mat getData() override;

private:
    cv::VideoCapture m_videoCapture;
};

}

