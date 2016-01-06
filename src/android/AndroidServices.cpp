#include "AndroidServices.h"

namespace {

JNIEnv* s_env = nullptr;
AAssetManager* s_assetManager = nullptr;

}

namespace yarrar {
namespace android {

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

}
}
