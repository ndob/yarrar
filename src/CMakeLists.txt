if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(UNITY_PLUGIN)
    set(SOURCE_FILES
        yarrar/tracker/marker/MarkerDetector.cpp
        yarrar/tracker/marker/MarkerDetector.h
        yarrar/tracker/marker/YarrarMarkerParser.cpp
        yarrar/tracker/marker/YarrarMarkerParser.h
        yarrar/tracker/marker/MarkerTracker.cpp
        yarrar/tracker/marker/MarkerTracker.h
        yarrar/Types.h
        yarrar/Util.h
        yarrar/Util.cpp
        unity/UnityNativePlugin.cpp)
else()
    set(SOURCE_FILES
        yarrar/tracker/marker/MarkerDetector.cpp
        yarrar/tracker/marker/MarkerDetector.h
        yarrar/tracker/marker/YarrarMarkerParser.cpp
        yarrar/tracker/marker/YarrarMarkerParser.h
        yarrar/tracker/marker/MarkerTracker.cpp
        yarrar/tracker/marker/MarkerTracker.h
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
        yarrar/Util.h
        yarrar/Util.cpp
        yarrar/Types.h)

    if(ANDROID)
        set(SOURCE_FILES
            ${SOURCE_FILES}
            android/YarrarGlue.cpp)
    endif()

endif()

add_library(yarrar ${SOURCE_FILES})

if(ANDROID)
    add_definitions(-DYARRAR_OPENGLES_CONTEXT)

    include_directories(
        yarrar 
        "${PROJECT_SOURCE_DIR}/external/OpenCV-android-sdk/sdk/native/jni/include/"
        "${PROJECT_SOURCE_DIR}/src"
        "${PROJECT_SOURCE_DIR}/external/json11")

    target_link_libraries(yarrar json11 ${OpenCV_LIBS} GLESv2)    
elseif(UNITY_PLUGIN)
    add_definitions(-DYARRAR_OPENGL_CONTEXT)

    include_directories(
        yarrar 
        "${PROJECT_SOURCE_DIR}/external/opencv/build/include"
        "${PROJECT_SOURCE_DIR}/external/json11")

    target_link_libraries(yarrar ${OpenCV_LIBS})
else()
    add_definitions(-DYARRAR_OPENGL_CONTEXT)
    if(MSVC)

        include_directories(
            yarrar 
            "${PROJECT_SOURCE_DIR}/external/opencv/build/include" 
            "${PROJECT_SOURCE_DIR}/external/glfw/include"
            "${PROJECT_SOURCE_DIR}/external/glew/include"
            "${PROJECT_SOURCE_DIR}/external/json11")

        target_link_libraries(
            yarrar opengl32 
            "${PROJECT_SOURCE_DIR}/external/glfw/lib-vc2015/glfw3.lib"
            "${PROJECT_SOURCE_DIR}/external/glew/lib/Release/x64/glew32.lib"
            ${OpenCV_LIBS})
    else()

        include_directories(
            yarrar
            "${PROJECT_SOURCE_DIR}/external/json11")

        target_link_libraries(yarrar ${OpenCV_LIBS} GL GLEW glfw)
    endif()
endif()
