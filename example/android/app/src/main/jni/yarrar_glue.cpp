#include <jni.h>
#include <android/log.h>

#include "yarrar/Pipeline.h"
#include "yarrar/dataprovider/StaticImageDataProvider.h"
#include "yarrar/detector/MarkerDetector.h"
#include "yarrar/renderer/opengl/OpenGLRenderer.h"

#define APPNAME "Yarrar"

yarrar::Pipeline* pipe;
cv::Mat s_image;

namespace yarrar {

class AndroidImageProvider: public DataProvider
{
public:
    AndroidImageProvider() {};
    cv::Mat getData() override
    {
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
jint Java_com_github_ndob_yarrarandroidexample_MainActivity_initYarrar(JNIEnv*, jobject)
{
    using namespace yarrar;
    pipe = new Pipeline;
    pipe->addDataProvider<AndroidImageProvider>();
    pipe->addDetector<MarkerDetector>();
    pipe->addRenderer<OpenGLRenderer>();

    cv::RNG m_rng(1423);
    return m_rng.uniform(0,255);
}

jint Java_com_github_ndob_yarrarandroidexample_MainActivity_run(JNIEnv*, jobject)
{
    pipe->run();
    return 0;
}

jint Java_com_github_ndob_yarrarandroidexample_MainActivity_runRender(JNIEnv*, jobject)
{
    pipe->runRender();
    return 0;
}

void Java_com_github_ndob_yarrarandroidexample_MainActivity_injectCameraFrame(JNIEnv* env, jobject, jint width, jint height, jbyteArray cameraData)
{
    jbyte* buffer = env->GetByteArrayElements(cameraData, nullptr);
    jsize len = env->GetArrayLength(cameraData);

    cv::Mat yuv(height + (height / 2), width, CV_8UC1, buffer);
    cv::Mat rgba(height, width, CV_8UC3);
    cv::cvtColor(yuv, rgba, cv::COLOR_YUV2BGR_NV21);

    s_image = rgba;

    env->ReleaseByteArrayElements(cameraData, buffer, JNI_ABORT);
}

}
