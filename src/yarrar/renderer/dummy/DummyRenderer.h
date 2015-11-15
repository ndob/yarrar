#pragma once

#include "Pipeline.h"

#include <iostream>

namespace yarrar {

class DummyRenderer : public Renderer
{
public:
    DummyRenderer(int width, int height) {};
    void draw(const yarrar::Pose& cameraPose, const cv::Mat& rawData) override
    {
        std::cout << "[DUMMY] translation: " << cameraPose.translation << std::endl;
        std::cout << "[DUMMY] rotation: " << cameraPose.rotation << std::endl;
        std::cout << "[DUMMY] id:" << cameraPose.coordinateSystemId << std::endl;
        std::cout << "[DUMMY] valid:" << cameraPose.valid << std::endl;
    }
};

}


