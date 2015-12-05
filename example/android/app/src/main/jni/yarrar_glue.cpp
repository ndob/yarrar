#include "yarrar/Pipeline.h"
#include "yarrar/detector/MarkerDetector.h"
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

class AndroidDummyRenderer : public Renderer
{
public:
    AndroidDummyRenderer(int width, int height) {};
    void draw(const yarrar::Pose& cameraPose, const cv::Mat& rawData) override
    {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "[DUMMY] id: %d", cameraPose.coordinateSystemId);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "[DUMMY] valid: %d", (int) cameraPose.valid);
    }
};

}

extern "C" {
void Java_com_github_ndob_yarrarandroidexample_MainActivity_initYarrar(JNIEnv*, jobject)
{
    using namespace yarrar;
    s_pipe = new Pipeline;
    s_pipe->addDataProvider<AndroidImageProvider>();
    s_pipe->addDetector<MarkerDetector>();
    s_pipe->addRenderer<OpenGLRenderer>();
}

void Java_com_github_ndob_yarrarandroidexample_MainActivity_run(JNIEnv*, jobject)
{
    s_pipe->run();
}

void Java_com_github_ndob_yarrarandroidexample_MainActivity_injectCameraFrame(JNIEnv* env, jobject, jint width, jint height, jbyteArray cameraData)
{   
    jbyte* buffer = env->GetByteArrayElements(cameraData, nullptr);
    jsize len = env->GetArrayLength(cameraData);

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
