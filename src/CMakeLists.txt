if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(UNITY_PLUGIN)
    set(SOURCE_FILES
        yarrar/tracker/marker/MarkerDetector.cpp
        yarrar/tracker/marker/YarrarMarkerParser.cpp
        yarrar/tracker/marker/MarkerTracker.cpp
        yarrar/Util.cpp
        yarrar/io/FileSystem_std.cpp
        unity/UnityNativePlugin.cpp)
else()
    set(SOURCE_FILES
        yarrar/tracker/marker/MarkerDetector.cpp
        yarrar/tracker/marker/YarrarMarkerParser.cpp
        yarrar/tracker/marker/MarkerTracker.cpp
        yarrar/renderer/opengl/OpenGLRenderer.cpp
        yarrar/Pipeline.cpp
        yarrar/dataprovider/WebcamDataProvider.cpp
        yarrar/renderer/opengl/GLProgram.cpp
        yarrar/renderer/opengl/GLShader.cpp
        yarrar/renderer/opengl/BackgroundModel.cpp
        yarrar/renderer/opengl/SceneModel.cpp
        yarrar/dataprovider/StaticImageDataProvider.cpp
        yarrar/dataprovider/AndroidCameraProvider.cpp
        yarrar/dataprovider/AndroidSensorProvider.cpp
        yarrar/renderer/opengl/GLContext.cpp
        yarrar/Scene.cpp
        yarrar/Util.cpp
        yarrar/tracker/sensor/SensorTracker.cpp
        yarrar/fusion/vsfusion/VisualWithSensors.cpp)

    if(ANDROID)
        set(SOURCE_FILES
            ${SOURCE_FILES}
            android/YarrarGlue.cpp
            android/AndroidServices.cpp
            yarrar/io/FileSystem_android.cpp)
    else()
        set(SOURCE_FILES
            ${SOURCE_FILES}
            yarrar/io/FileSystem_std.cpp)
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

    target_link_libraries(yarrar json11 ${OpenCV_LIBS} GLESv2 android)
elseif(UNITY_PLUGIN)
    add_definitions(-DYARRAR_OPENGL_CONTEXT)

    include_directories(
        yarrar 
        "${PROJECT_SOURCE_DIR}/external/opencv/build/include"
        "${PROJECT_SOURCE_DIR}/external/json11")

    target_link_libraries(yarrar json11 ${OpenCV_LIBS})
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
            "${PROJECT_SOURCE_DIR}/external/json11"
            "${PROJECT_SOURCE_DIR}/external/eigen")

        target_link_libraries(yarrar ${OpenCV_LIBS} GL GLEW glfw)
    endif()
endif()
