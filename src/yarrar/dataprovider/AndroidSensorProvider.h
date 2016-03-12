#pragma once

#include "PipelineStage.h"

namespace yarrar
{

class AndroidSensorProvider : public DataProvider
{
public:
    AndroidSensorProvider(const json11::Json& config);
    const LockableData<Datapoint>& getData() override;
    Dimensions getDimensions() override;
    DatatypeFlags provides() override;

    static void injectRotation(const std::vector<float>& quaternion);

private:
    static LockableData<Datapoint> s_dp;
};
}
