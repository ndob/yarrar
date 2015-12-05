#include "yarrar/Pipeline.h"
#include "yarrar/dataprovider/WebcamDataProvider.h"
#include "yarrar/detector/MarkerDetector.h"
#include "yarrar/renderer/opengl/OpenGLRenderer.h"

int main(int argc, char** argv)
{
    using namespace yarrar;

    Pipeline p;
    p.addDataProvider<WebcamDataProvider>();
    p.addDetector<MarkerDetector>();
    p.addRenderer<OpenGLRenderer>();

    while(true)
    {
        p.run();
        p.runRender();
    }
}

