include_directories("${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/src/yarrar")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)

add_executable(yarrar ${SOURCE_FILES})
find_package(OpenCV REQUIRED)
add_definitions(-DYARRAR_OPENGL_CONTEXT)
target_link_libraries(yarrar libyarrar ${OpenCV_LIBS} GL GLEW glfw)
