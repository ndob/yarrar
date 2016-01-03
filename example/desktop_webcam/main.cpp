#include "yarrar/Pipeline.h"
#include "yarrar/dataprovider/WebcamDataProvider.h"
#include "yarrar/tracker/marker/MarkerTracker.h"
#include "yarrar/tracker/marker/YarrarMarkerParser.h"
#include "yarrar/renderer/opengl/OpenGLRenderer.h"

int main(int argc, char** argv)
{
    using namespace yarrar;

    Pipeline p;
    p.addDataProvider<WebcamDataProvider>();
    p.addTracker<MarkerTracker<YarrarMarkerParser>>();
    p.addRenderer<OpenGLRenderer>();

    Model cube;
    cube.name = "cube";
    cube.translation = {0.0f, 0.0f, 0.0f};
    cube.vertices = {
        // bottom
        -1.0f,  -1.0f,  0.0f,
        1.0f,   -1.0f,  0.0f,
        -1.0f,  -1.0f,  2.0f,
        1.0f,   -1.0f,  0.0f,
        1.0f,   -1.0f,  2.0f,
        -1.0f,  -1.0f,  2.0f,

        // top
        -1.0f,  1.0f,   0.0f,
        -1.0f,  1.0f,   2.0f,
        1.0f,   1.0f,   0.0f,
        1.0f,   1.0f,   0.0f,
        -1.0f,  1.0f,   2.0f,
        1.0f,   1.0f,   2.0f,

        // front
        -1.0f,  -1.0f,  2.0f,
        1.0f,   -1.0f,  2.0f,
        -1.0f,  1.0f,   2.0f,
        1.0f,   -1.0f,  2.0f,
        1.0f,   1.0f,   2.0f,
        -1.0f,  1.0f,   2.0f,

        // back
        -1.0f,  -1.0f,  0.0f,
        -1.0f,  1.0f,   0.0f,
        1.0f,   -1.0f,  0.0f,
        1.0f,   -1.0f,  0.0f,
        -1.0f,  1.0f,   0.0f,
        1.0f,   1.0f,   0.0f,

        // left
        -1.0f,  -1.0f,  2.0f,
        -1.0f,  1.0f,   0.0f,
        -1.0f,  -1.0f,  0.0f,
        -1.0f,  -1.0f,  2.0f,
        -1.0f,  1.0f,   2.0f,
        -1.0f,  1.0f,   0.0f,

        // right
        1.0f,   -1.0f,  2.0f,
        1.0f,   -1.0f,  0.0f,
        1.0f,   1.0f,   0.0f,
        1.0f,   -1.0f,  2.0f,
        1.0f,   1.0f,   0.0f,
        1.0f,   1.0f,   2.0
    };

    p.addModel(cube);

    while(true)
    {
        p.run();
    }

}

