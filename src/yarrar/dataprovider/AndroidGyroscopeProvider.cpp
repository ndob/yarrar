#include "AndroidGyroscopeProvider.h"
#include "android/AndroidServices.h"

namespace yarrar
{

LockableData<Datapoint> AndroidGyroscopeProvider::s_dp({});

AndroidGyroscopeProvider::AndroidGyroscopeProvider(const json11::Json& config)
    : DataProvider(config)
{
}

const LockableData<Datapoint>& AndroidGyroscopeProvider::getData()
{
    return s_dp;
}

yarrar::Dimensions AndroidGyroscopeProvider::getDimensions()
{
    return yarrar::Dimensions();
}

DatatypeFlags AndroidGyroscopeProvider::provides()
{
    return GYROSCOPE_SENSOR_FLAG;
}

void AndroidGyroscopeProvider::injectGyroscope(const std::vector<float>& data)
{
    yarrar::android::log("Got new gyroscope data.");
    auto handle = s_dp.lockReadWrite();
    handle.set({ TimestampClock::now(),
        cv::Mat(data, true) });
}
}
