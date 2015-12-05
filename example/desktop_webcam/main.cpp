#include "yarrar/Pipeline.h"
#include "yarrar/dataprovider/WebcamDataProvider.h"
#include "yarrar/dataprovider/StaticImageDataProvider.h"
#include "yarrar/detector/MarkerDetector.h"
#include "yarrar/renderer/opengl/OpenGLRenderer.h"
#include "yarrar/renderer/dummy/DummyRenderer.h"
#include "yarrar/renderer/opencv/OpenCVRenderer.h"

int main(int argc, char** argv)
{
    using namespace yarrar;

    Pipeline p;
    p.addDataProvider<WebcamDataProvider>();
    //p.addDataProvider<StaticImageDataProvider>("/home/niki/programming/yarrar/data/img/marker.jpg");
    p.addDetector<MarkerDetector>();
    //p.addRenderer<DummyRenderer>();
    //p.addRenderer<OpenCVRenderer>();
    p.addRenderer<OpenGLRenderer>();

    while(true)
    {
        p.run();
        p.runRender();
        //std::getchar();
    }
}

