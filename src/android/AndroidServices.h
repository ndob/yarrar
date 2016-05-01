#pragma once

#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <jni.h>

#include <string>

namespace yarrar
{
namespace android
{

// Android sensor constants
// See: http://developer.android.com/reference/android/hardware/Sensor.html
enum SensorType
{
    TYPE_ROTATION_VECTOR = 0x0000000b,
    TYPE_GYROSCOPE = 0x00000004
};


bool initialize(JNIEnv* env, jobject yarrarActivity, jobject assetManager);
AAssetManager* getAssetManager();
bool setSensorState(SensorType type, bool active);
void log(const std::string& message);
}
}
