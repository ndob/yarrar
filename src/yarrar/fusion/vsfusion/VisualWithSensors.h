#pragma once

#include "PipelineStage.h"

namespace yarrar
{

class VisualWithSensors : public SensorFusion
{
public:
    VisualWithSensors(const json11::Json& config);
    void getFusedPoses(const std::map<size_t, std::vector<Pose>>& poseLists,
        std::vector<Pose>& output) override;
};
}
