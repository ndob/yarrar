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

add_library(yarrar SHARED ${SOURCE_FILES})

if(ANDROID)
        add_definitions(-DYARRAR_OPENGLES_CONTEXT)
        include_directories(yarrar "${PROJECT_SOURCE_DIR}/external/OpenCV-android-sdk/sdk/native/jni/include/")
        target_link_libraries(yarrar ${OpenCV_LIBS} GLESv2)
else()
        add_definitions(-DYARRAR_OPENGL_CONTEXT)
        include_directories(yarrar)
        target_link_libraries(yarrar ${OpenCV_LIBS} GL GLEW glfw)
endif()
