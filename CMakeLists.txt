cmake_minimum_required(VERSION 3.2)
project(yarrar)
enable_testing()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

set("OpenCV_DIR" "${PROJECT_SOURCE_DIR}/external/OpenCV-android-sdk/sdk/native/jni/")
find_package(OpenCV REQUIRED)

add_subdirectory(src)
