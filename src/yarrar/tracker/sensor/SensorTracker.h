#pragma once

#include "PipelineStage.h"

namespace yarrar
{

class SensorTracker : public Tracker
{
public:
    SensorTracker(int width, int height, const json11::Json& config);
    DatatypeFlags depends() override;
    void getPoses(const Datapoint& data, std::vector<Pose>& output) override;
};
}
