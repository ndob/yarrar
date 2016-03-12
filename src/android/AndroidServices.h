#pragma once

#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <jni.h>

#include <string>

namespace yarrar
{
namespace android
{

bool initialize(JNIEnv* env, jobject assetManager);
AAssetManager* getAssetManager();
void log(const std::string& message);
}
}
