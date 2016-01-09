#pragma once

#include "PipelineStage.h"
#include "Scene.h"
#include "Types.h"
#include "Util.h"

#include <json11.hpp>
#include <memory>
#include <vector>
#include <opencv2/core/mat.hpp>

namespace yarrar
{

class Pipeline
{
public:
    explicit Pipeline(const std::string& configFile);

    void addModel(const Model& model);
    void run() const;

private:
    void validate();

    template <typename T>
    void addDataProvider(const json11::Json& config)
    {
        m_dataProviders.emplace_back(new T(config));
    }

    template <typename T>
    void addTracker(const json11::Json& config)
    {
        auto dim = m_dataProviders[0]->getDimensions();
        m_trackers.emplace_back(new T(dim.width, dim.height, config));
    }

    template <typename T>
    void addRenderer(const json11::Json& config)
    {
        auto dim = m_dataProviders[0]->getDimensions();
        m_renderers.emplace_back(new T(dim.width, dim.height, config));
    }

    std::vector<std::unique_ptr<DataProvider>> m_dataProviders;
    std::vector<std::unique_ptr<Tracker>> m_trackers;
    std::vector<std::unique_ptr<Renderer>> m_renderers;
    Scene m_scene;
};
}
