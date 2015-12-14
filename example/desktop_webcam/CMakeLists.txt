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
target_link_libraries(example_desktop_webcam yarrar "${PROJECT_SOURCE_DIR}/external/glew/lib/Release/Win32/glew32.lib" ${OpenCV_LIBS})
