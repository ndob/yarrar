#include "VisualWithSensors.h"

namespace yarrar
{

VisualWithSensors::VisualWithSensors(const json11::Json& config)
    : SensorFusion(config)
{
}

void VisualWithSensors::getFusedPoses(const std::map<size_t, std::vector<Pose>>& poseLists,
    std::vector<Pose>& output)
{
}
}
