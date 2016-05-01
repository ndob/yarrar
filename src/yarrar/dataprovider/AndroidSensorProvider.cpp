#include "AndroidSensorProvider.h"

#include "android/AndroidServices.h"

namespace yarrar
{

LockableData<Datapoint> AndroidSensorProvider::s_dp({});

AndroidSensorProvider::AndroidSensorProvider(const json11::Json& config)
    : DataProvider(config)
{
    android::setSensorState(android::SensorType::TYPE_ROTATION_VECTOR, true);
}

const LockableData<yarrar::Datapoint>& yarrar::AndroidSensorProvider::getData()
{
    return s_dp;
}

yarrar::Dimensions AndroidSensorProvider::getDimensions()
{
    return yarrar::Dimensions();
}

yarrar::DatatypeFlags AndroidSensorProvider::provides()
{
    return ORIENTATION_SENSOR_FLAG;
}

void AndroidSensorProvider::injectRotation(const std::vector<float>& quaternion)
{
    assert(quaternion.size() == 4);

    auto handle = s_dp.lockReadWrite();
    handle.set({ TimestampClock::now(),
        cv::Mat(quaternion, true) });
}
}
