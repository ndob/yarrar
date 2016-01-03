#pragma once

#include "Pipeline.h"

#include <opencv2/videoio.hpp>

namespace yarrar
{

class StaticImageDataProvider: public DataProvider
{
public:
    StaticImageDataProvider(const json11::Json& config);
    cv::Mat getData() override;
    Dimensions getDimensions() override;
    DatatypeFlags provides() override;

private:
    cv::Mat m_cachedImage;
};

}

