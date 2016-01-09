#include "AndroidServices.h"
#include "yarrar/Pipeline.h"
#include "yarrar/dataprovider/AndroidImageProvider.h"

#include <jni.h>
#include <android/log.h>

namespace
{

yarrar::Pipeline* s_pipe;
}

extern "C" {

void Java_com_ndob_yarrar_YarrarActivity_initYarrar(JNIEnv* env, jobject, jint width, jint height, jobject assetManager)
{
    yarrar::android::initialize(env, assetManager);
    yarrar::AndroidImageProvider::injectCameraSize(static_cast<int>(width), static_cast<int>(height));
    s_pipe = new yarrar::Pipeline("pipeline.json");
}

void Java_com_ndob_yarrar_YarrarActivity_deinitYarrar(JNIEnv*, jobject)
{
    delete s_pipe;
}

void Java_com_ndob_yarrar_YarrarActivity_addModel(JNIEnv* env, jobject, jfloatArray vertices)
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
    s_pipe->addModel(cube);
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
    yarrar::AndroidImageProvider::injectCameraFrame(rgb);

    env->ReleaseByteArrayElements(cameraData, buffer, JNI_ABORT);
}
}
