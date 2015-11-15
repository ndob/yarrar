#include "Pipeline.h"

namespace yarrar
{

void Pipeline::run() const
{
    auto rawData = m_dataProviders[0]->getData();
    auto pose = m_detectors[0]->getPose(rawData);
    for(const auto& renderer : m_renderers)
    {
        renderer->draw(pose, rawData);
    }
}

}

