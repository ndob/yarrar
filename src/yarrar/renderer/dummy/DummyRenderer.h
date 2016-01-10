#pragma once

#include "Pipeline.h"

#include <iostream>

namespace yarrar
{

class DummyRenderer : public Renderer
{
public:
    DummyRenderer(int width, int height, const json11::Json& config)
        : Renderer(config){};

    void loadModel(const Model& model) override
    {
    }

    void draw(const std::vector<Pose>& cameraPoses,
        const Scene& scene,
        const Datapoint& rawData) override
    {
        for(const auto& pose : cameraPoses)
        {
            std::cout << "[DUMMY] New camera pose" << std::endl;
            std::cout << "[DUMMY] translation: " << pose.translation << std::endl;
            std::cout << "[DUMMY] rotation: " << pose.rotation << std::endl;
            std::cout << "[DUMMY] id:" << pose.coordinateSystemId << std::endl;
        }
    }
};
}
