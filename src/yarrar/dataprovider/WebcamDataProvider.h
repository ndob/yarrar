#pragma once

#include "Pipeline.h"

#include <opencv2/videoio.hpp>

namespace yarrar
{

class WebcamDataProvider : public DataProvider
{
public:
    WebcamDataProvider(const json11::Json& config);
    const LockableData<Datapoint>& getData() override;
    Dimensions getDimensions() override;
    DatatypeFlags provides() override;

private:
    cv::VideoCapture m_videoCapture;
    LockableData<Datapoint> m_dp;
};
}
