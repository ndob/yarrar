#pragma once

#include "Pipeline.h"

#include <iostream>

namespace yarrar {

class DummyRenderer : public Renderer
{
public:
    DummyRenderer(int width, int height) {};

    void loadModel(const Model& model) override
    {
    }

    void draw(const Pose& cameraPose,
              const Scene& scene,
              const cv::Mat& backgroundImage) override
    {
        std::cout << "[DUMMY] translation: " << cameraPose.translation << std::endl;
        std::cout << "[DUMMY] rotation: " << cameraPose.rotation << std::endl;
        std::cout << "[DUMMY] id:" << cameraPose.coordinateSystemId << std::endl;
        std::cout << "[DUMMY] valid:" << cameraPose.valid << std::endl;
    }
};

}


