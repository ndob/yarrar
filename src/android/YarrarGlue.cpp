#include "AndroidServices.h"
#include "yarrar/Pipeline.h"
#include "yarrar/dataprovider/AndroidCameraProvider.h"
#include "yarrar/dataprovider/AndroidGyroscopeProvider.h"
#include "yarrar/dataprovider/AndroidSensorProvider.h"

#include <jni.h>

#include <vector>

namespace
{

yarrar::Pipeline* s_pipe;
}

extern "C" {

void Java_com_ndob_yarrar_YarrarActivity_initYarrar(JNIEnv* env, jobject, jint width, jint height, jobject assetManager)
{
    try
    {
        yarrar::android::initialize(env, assetManager);
        yarrar::AndroidCameraProvider::injectCameraSize(static_cast<int>(width), static_cast<int>(height));
        s_pipe = new yarrar::Pipeline("pipeline.json");
    }
    catch(const std::exception& error)
    {
        yarrar::android::log(std::string("Error initializing yarrar-native:") + error.what());
    }
}

void Java_com_ndob_yarrar_YarrarActivity_deinitYarrar(JNIEnv*, jobject)
{
    delete s_pipe;
}

void Java_com_ndob_yarrar_YarrarActivity_addModel(JNIEnv* env, jobject, jint coordinateSystemId, jfloatArray vertices)
{
    yarrar::Model cube;
    cube.name = "cube";
    cube.translation = { 0.0f, 0.0f, 0.0f };

    jfloat* vertexArray = env->GetFloatArrayElements(vertices, nullptr);
    jsize len = env->GetArrayLength(vertices);

    for(jsize i = 0; i < len; ++i)
    {
        cube.vertices.push_back(static_cast<float>(vertexArray[i]));
    }

    env->ReleaseFloatArrayElements(vertices, vertexArray, JNI_ABORT);
    s_pipe->addModel(coordinateSystemId, cube);
}

void Java_com_ndob_yarrar_YarrarActivity_run(JNIEnv*, jobject)
{
    s_pipe->run();
}

void Java_com_ndob_yarrar_YarrarActivity_injectCameraFrame(JNIEnv* env, jobject, jint width, jint height, jbyteArray cameraData)
{
    jbyte* buffer = env->GetByteArrayElements(cameraData, nullptr);

    cv::Mat yuv(height + (height / 2), width, CV_8UC1, buffer);
    cv::Mat rgb(height, width, CV_8UC3);
    cv::cvtColor(yuv, rgb, cv::COLOR_YUV2BGR_NV21);
    yarrar::util::rotate(rgb, rgb, yarrar::Rotation90::DEG_90);
    yarrar::AndroidCameraProvider::injectCameraFrame(rgb);

    env->ReleaseByteArrayElements(cameraData, buffer, JNI_ABORT);
}

void Java_com_ndob_yarrar_YarrarActivity_injectSensorRotation(JNIEnv* env, jobject, jfloatArray quaternion)
{

    jfloat* quaternionArray = env->GetFloatArrayElements(quaternion, nullptr);
    jsize len = env->GetArrayLength(quaternion);

    std::vector<float> retQuaternion;
    retQuaternion.reserve(len);
    for(jsize i = 0; i < len; ++i)
    {
        retQuaternion.push_back(static_cast<float>(quaternionArray[i]));
    }
    yarrar::AndroidSensorProvider::injectRotation(retQuaternion);
    env->ReleaseFloatArrayElements(quaternion, quaternionArray, JNI_ABORT);
}

void Java_com_ndob_yarrar_YarrarActivity_injectSensorGyroscope(JNIEnv* env, jobject, jfloatArray values)
{
    jfloat* valueArray = env->GetFloatArrayElements(values, nullptr);
    jsize len = env->GetArrayLength(values);

    std::vector<float> ret;
    ret.reserve(len);
    for(jsize i = 0; i < len; ++i)
    {
        ret.push_back(static_cast<float>(valueArray[i]));
    }
    yarrar::AndroidGyroscopeProvider::injectGyroscope(ret);
    env->ReleaseFloatArrayElements(values, valueArray, JNI_ABORT);
}
}
