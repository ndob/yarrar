cmake_minimum_required(VERSION 3.3)
project(yarrar)

find_package( OpenCV REQUIRED )
include_directories(src src/yarrar)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES
        src/example/main.cpp
        src/yarrar/detector/MarkerDetector.cpp
        src/yarrar/detector/MarkerDetector.h
        src/yarrar/Pose.h
        src/yarrar/renderer/opengl/OpenGLRenderer.cpp
        src/yarrar/renderer/opengl/OpenGLRenderer.h
        src/yarrar/Pipeline.cpp
        src/yarrar/Pipeline.h
        src/yarrar/dataprovider/WebcamDataProvider.cpp
        src/yarrar/dataprovider/WebcamDataProvider.h
        src/yarrar/renderer/opengl/GLProgram.cpp
        src/yarrar/renderer/opengl/GLProgram.h
        src/yarrar/renderer/opengl/GLShader.cpp
        src/yarrar/renderer/opengl/GLShader.h
        src/yarrar/renderer/opengl/GLModel.h
        src/yarrar/renderer/opengl/BackgroundModel.cpp
        src/yarrar/renderer/opengl/BackgroundModel.h
        src/yarrar/renderer/opengl/CubeModel.cpp
        src/yarrar/renderer/opengl/CubeModel.h
        src/yarrar/dataprovider/StaticImageDataProvider.cpp
        src/yarrar/dataprovider/StaticImageDataProvider.h
        src/yarrar/renderer/dummy/DummyRenderer.h
        src/yarrar/renderer/opencv/OpenCVRenderer.h)

add_executable(yarrar ${SOURCE_FILES})
target_link_libraries( yarrar ${OpenCV_LIBS} GL GLEW glfw)