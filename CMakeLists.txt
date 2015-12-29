if(MSVC)
    # 3.4 is needed for CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS
    cmake_minimum_required(VERSION 3.4) 
else()
    cmake_minimum_required(VERSION 3.2)
endif()

project(yarrar)
enable_testing()

option(UNITY_PLUGIN "Build plugin for Unity" OFF)
option(BUILD_EXAMPLES "Build example applications" ON)
option(BUILD_TESTS "Build tests" ON)

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
    include("${PROJECT_SOURCE_DIR}/cmake/windows/PowerShellDownloadDependencies.cmake")
    set("OpenCV_DIR" "${PROJECT_SOURCE_DIR}/external/opencv/build")
endif()

find_package(OpenCV REQUIRED)

add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(example/desktop_webcam)
endif()

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
