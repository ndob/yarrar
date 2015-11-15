#pragma once

#include "Pipeline.h"

namespace yarrar {

class OpenCVRenderer : public Renderer
{
public:
    OpenCVRenderer(int width, int height)
    {
        cv::namedWindow("OpenCVRenderer", 1);
    };

    void draw(const yarrar::Pose& cameraPose, const cv::Mat& rawData) override
    {
        cv::imshow("OpenCVRenderer", rawData);
        cv::waitKey(30);
    }
};

}