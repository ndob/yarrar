#include "Pipeline.h"

namespace yarrar
{

void Pipeline::run() const
{
    auto rawData = m_dataProviders[0]->getData();
    if(rawData.total() == 0) return;

    std::vector<Pose> poses;
    for(const auto& tracker : m_trackers)
    {
        tracker->getPoses(rawData, poses);
    }

    for(const auto& renderer : m_renderers)
    {
        renderer->draw(poses, m_scene, rawData);
    }
}

void Pipeline::addModel(const Model& model)
{
    m_scene.addModel(model);
    for(const auto& renderer : m_renderers)
    {
        renderer->loadModel(model);
    }
}

}

