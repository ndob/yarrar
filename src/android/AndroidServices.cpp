#include "AndroidServices.h"

#include <android/log.h>

namespace
{

JNIEnv* s_env = nullptr;
jobject s_yarrarActivity = nullptr;
AAssetManager* s_assetManager = nullptr;
}

namespace yarrar
{
namespace android
{

bool initialize(JNIEnv* env, jobject yarrarActivity, jobject assetManager)
{
    s_env = env;
    s_yarrarActivity = yarrarActivity;
    s_assetManager = AAssetManager_fromJava(env, assetManager);
    return s_env != nullptr && s_assetManager != nullptr;
}

AAssetManager* getAssetManager()
{
    return s_assetManager;
}

bool setSensorState(SensorType type, bool active)
{
    jclass clazz = s_env->GetObjectClass(s_yarrarActivity);
    jmethodID methodId = s_env->GetMethodID(clazz, "native_setSensorState", "(IZ)Z");
    if(methodId == 0)
    {
        log("Could not find activateSensor-method through JNI.");
        return false;
    }

    jboolean ret = s_env->CallBooleanMethod(s_yarrarActivity, methodId, static_cast<int>(type), active);
    return static_cast<bool>(ret);
}

void log(const std::string& message)
{
    __android_log_print(ANDROID_LOG_VERBOSE, "YARRAR", message.c_str());
}
}
}
