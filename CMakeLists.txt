cmake_minimum_required(VERSION 3.2)
project(yarrar)
enable_testing()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_USE_MATH_DEFINES")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(ANDROID)
    set("OpenCV_DIR" "${PROJECT_SOURCE_DIR}/external/opencv/sources")
elseif(MSVC)
    set("OpenCV_DIR" "${PROJECT_SOURCE_DIR}/external/opencv/sources/local_build")
    add_subdirectory(${PROJECT_SOURCE_DIR}/external/glfw)
endif()

find_package(OpenCV REQUIRED)

add_subdirectory(src)
if(NOT DEFINED ANDROID)
    add_subdirectory(example/desktop_webcam)
    add_subdirectory(test)
endif()
