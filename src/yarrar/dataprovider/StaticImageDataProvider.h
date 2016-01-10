#pragma once

#include "Pipeline.h"

#include <opencv2/videoio.hpp>

namespace yarrar
{

class StaticImageDataProvider : public DataProvider
{
public:
    StaticImageDataProvider(const json11::Json& config);
    const LockableData<Datapoint>& getData() override;
    Dimensions getDimensions() override;
    DatatypeFlags provides() override;

private:
    LockableData<Datapoint> m_dp;
    Dimensions m_imageDimensions;
};
}
