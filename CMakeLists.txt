cmake_minimum_required(VERSION 3.2)
project(yarrar)
enable_testing()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

find_package(OpenCV REQUIRED)

add_subdirectory(src)
add_subdirectory(example/desktop_webcam)
add_subdirectory(test)

