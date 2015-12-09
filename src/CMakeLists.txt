include_directories(yarrar)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES
        yarrar/detector/MarkerDetector.cpp
        yarrar/detector/MarkerDetector.h
        yarrar/Pose.h
        yarrar/renderer/opengl/OpenGLRenderer.cpp
        yarrar/renderer/opengl/OpenGLRenderer.h
        yarrar/Pipeline.cpp
        yarrar/Pipeline.h
        yarrar/dataprovider/WebcamDataProvider.cpp
        yarrar/dataprovider/WebcamDataProvider.h
        yarrar/renderer/opengl/GLProgram.cpp
        yarrar/renderer/opengl/GLProgram.h
        yarrar/renderer/opengl/GLShader.cpp
        yarrar/renderer/opengl/GLShader.h
        yarrar/renderer/opengl/BackgroundModel.cpp
        yarrar/renderer/opengl/BackgroundModel.h
        yarrar/renderer/opengl/SceneModel.cpp
        yarrar/renderer/opengl/SceneModel.h
        yarrar/dataprovider/StaticImageDataProvider.cpp
        yarrar/dataprovider/StaticImageDataProvider.h
        yarrar/renderer/dummy/DummyRenderer.h
        yarrar/renderer/opencv/OpenCVRenderer.h
        yarrar/renderer/opengl/GLContext.cpp
        yarrar/renderer/opengl/GLContext.h
        yarrar/Scene.cpp
        yarrar/Scene.h
        yarrar/Util.h)

add_definitions(-DYARRAR_OPENGL_CONTEXT)
add_library(libyarrar ${SOURCE_FILES})
