#include "AndroidServices.h"

#include <android/log.h>

namespace
{

JNIEnv* s_env = nullptr;
AAssetManager* s_assetManager = nullptr;
}

namespace yarrar
{
namespace android
{

bool initialize(JNIEnv* env, jobject assetManager)
{
    s_env = env;
    s_assetManager = AAssetManager_fromJava(env, assetManager);
    return s_env != nullptr && s_assetManager != nullptr;
}

AAssetManager* getAssetManager()
{
    return s_assetManager;
}

void log(const std::string& message)
{
    __android_log_print(ANDROID_LOG_VERBOSE, "YARRAR", message.c_str());
}
}
}
