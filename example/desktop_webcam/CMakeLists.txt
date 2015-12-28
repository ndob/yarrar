include_directories(
    "${PROJECT_SOURCE_DIR}/src" 
    "${PROJECT_SOURCE_DIR}/src/yarrar" 
    "${PROJECT_SOURCE_DIR}/external/opencv/build/include"
    "${PROJECT_SOURCE_DIR}/external/glfw/include"
    "${PROJECT_SOURCE_DIR}/external/glew/include"
)

set(SOURCE_FILES main.cpp)
add_executable(example_desktop_webcam ${SOURCE_FILES})
add_definitions(-DYARRAR_OPENGL_CONTEXT)
target_link_libraries(example_desktop_webcam yarrar ${OpenCV_LIBS})

#TODO: Move to a separate file as this is used also by testrunner
if(MSVC)
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/external/glew/bin/Release/x64/*.dll" GLEW_DLL_FILES)
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/external/glfw/lib-vc2015/*.dll" GLFW_DLL_FILES)
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/external/opencv/build/x64/vc14/bin" OPENCV_DLL_FILES)
    file(TO_NATIVE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>" MSVC_BINARY_OUTPUT_PATH)

    add_custom_command(TARGET example_desktop_webcam POST_BUILD
        COMMAND copy "${GLEW_DLL_FILES}" "${MSVC_BINARY_OUTPUT_PATH}")
    add_custom_command(TARGET example_desktop_webcam POST_BUILD
        COMMAND copy "${GLFW_DLL_FILES}" "${MSVC_BINARY_OUTPUT_PATH}")
    add_custom_command(TARGET example_desktop_webcam POST_BUILD
        COMMAND copy "${OPENCV_DLL_FILES}" "${MSVC_BINARY_OUTPUT_PATH}")
endif()
