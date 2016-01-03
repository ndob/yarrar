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

    void loadModel(const Model& model) override
    {
    }

    void draw(const std::vector<Pose>& cameraPoses,
              const Scene& scene,
              const cv::Mat& backgroundImage) override
    {
        cv::imshow("OpenCVRenderer", backgroundImage);
        cv::waitKey(30);
    }
};

}