#include "Pipeline.h"

namespace yarrar
{

static Pose lastPose;
static cv::Mat lastData;

void Pipeline::run() const
{
    auto rawData = m_dataProviders[0]->getData();
    auto pose = m_detectors[0]->getPose(rawData);
    lastPose = pose;
    lastData = rawData;
    for(const auto& renderer : m_renderers)
    {
        //renderer->draw(pose, rawData);
    }
}

void Pipeline::runRender() const
{
    m_renderers[0]->draw(lastPose, lastData);
}

}

