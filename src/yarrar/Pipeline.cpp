#include "Pipeline.h"

namespace yarrar
{

void Pipeline::run() const
{
    auto rawData = m_dataProviders[0]->getData();
    if(rawData.total() == 0) return;

    auto pose = m_trackers[0]->getPose(rawData);
    for(const auto& renderer : m_renderers)
    {
        renderer->draw(pose, m_scene, rawData);
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

