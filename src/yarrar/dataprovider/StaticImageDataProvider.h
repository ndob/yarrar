#pragma once

#include "Pipeline.h"

#include <opencv2/videoio.hpp>

namespace yarrar
{

class StaticImageDataProvider: public DataProvider
{
public:
    StaticImageDataProvider(const std::string& imagePath);
    cv::Mat getData() override;

private:
    std::string m_imagePath;
    cv::Mat m_cachedImage;
};

}

