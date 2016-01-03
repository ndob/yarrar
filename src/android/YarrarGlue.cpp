#include "yarrar/Pipeline.h"
#include "yarrar/tracker/marker/MarkerTracker.h"
#include "yarrar/tracker/marker/YarrarMarkerParser.h"
#include "yarrar/renderer/opengl/OpenGLRenderer.h"

#include <jni.h>
#include <android/log.h>
#include <mutex>

#define APPNAME "Yarrar"

namespace {

yarrar::Pipeline* s_pipe;
cv::Mat s_image;
std::mutex s_imageMutex; 

}

namespace yarrar {

class AndroidImageProvider: public DataProvider
{
public:
    AndroidImageProvider() {};
    cv::Mat getData() override
    {
        std::lock_guard<std::mutex> m_(s_imageMutex);
        return s_image;
    }

    Dimensions getDimensions()
    {
        return {
            960, 720
        };
    }
};

}

extern "C" {

void Java_com_ndob_yarrar_YarrarActivity_initYarrar(JNIEnv*, jobject)
{
    using namespace yarrar;
    s_pipe = new Pipeline;
    s_pipe->addDataProvider<AndroidImageProvider>();
    s_pipe->addTracker<MarkerTracker<YarrarMarkerParser>>();
    s_pipe->addRenderer<OpenGLRenderer>();
}

void Java_com_ndob_yarrar_YarrarActivity_addModel(JNIEnv* env, jobject, jfloatArray vertices)
{
    yarrar::Model cube;
    cube.name = "cube";
    cube.translation = {0.0f, 0.0f, 0.0f};

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
    cv::Mat rgba(height, width, CV_8UC3);
    cv::cvtColor(yuv, rgba, cv::COLOR_YUV2BGR_NV21);

    {
        std::lock_guard<std::mutex> m_(s_imageMutex);
        s_image = rgba;
    }

    env->ReleaseByteArrayElements(cameraData, buffer, JNI_ABORT);
}

}
