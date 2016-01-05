#pragma once

#include <android/asset_manager_jni.h>
#include <jni.h>

namespace yarrar {
namespace android {

bool initialize(JNIEnv* env, jobject assetManager);
AAssetManager* getAssetManager();

}
}