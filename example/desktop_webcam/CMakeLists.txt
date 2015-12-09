include_directories("${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/src/yarrar")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)

add_executable(example_desktop_webcam ${SOURCE_FILES})

add_definitions(-DYARRAR_OPENGL_CONTEXT)
target_link_libraries(example_desktop_webcam libyarrar ${OpenCV_LIBS} GL GLEW glfw)
