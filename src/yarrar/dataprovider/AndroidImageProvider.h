#pragma once

#include "PipelineStage.h"

#include <json11.hpp>
#include <mutex>

namespace yarrar
{

class AndroidImageProvider : public DataProvider
{
public:
    AndroidImageProvider(const json11::Json& config);
    cv::Mat getData() override;
    Dimensions getDimensions() override;
    DatatypeFlags provides() override;

    static void injectCameraSize(const int width, const int height);
    static void injectCameraFrame(const cv::Mat& rgb);

private:
    static int s_width;
    static int s_height;
    static cv::Mat s_image;
    static std::mutex s_imageMutex;
};
}
