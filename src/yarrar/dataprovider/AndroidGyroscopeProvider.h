#pragma once

#include "PipelineStage.h"

namespace yarrar
{

class AndroidGyroscopeProvider : public DataProvider
{
public:
    AndroidGyroscopeProvider(const json11::Json& config);
    const LockableData<Datapoint>& getData() override;
    Dimensions getDimensions() override;
    DatatypeFlags provides() override;

    static void injectGyroscope(const std::vector<float>& data);

private:
    static LockableData<Datapoint> s_dp;
};
}
