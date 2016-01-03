#include "yarrar/Pipeline.h"


int main(int argc, char** argv)
{
    using namespace yarrar;

    std::string configFile = "../../pipeline.json";
    if(argc != 2)
    {
        std::cout << "no pipeline configuration file specified. using default" << std::endl;
    }
    else
    {
        configFile = argv[1];
    }

    Pipeline p(configFile);

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

