#pragma once

#include "PipelineStage.h"

#include <json11.hpp>
#include <mutex>

namespace yarrar {

class AndroidImageProvider: public DataProvider
{
public:
    AndroidImageProvider(const json11::Json& config);
    cv::Mat getData() override;
    Dimensions getDimensions() override;
    DatatypeFlags provides() override;

    static void injectCameraFrame(const cv::Mat& rgb);

private:

    static cv::Mat s_image;
    static std::mutex s_imageMutex;
};

}
