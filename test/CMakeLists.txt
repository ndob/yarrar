enable_testing()

include_directories(
    "${PROJECT_SOURCE_DIR}/src" 
    "${PROJECT_SOURCE_DIR}/src/yarrar"
    "${PROJECT_SOURCE_DIR}/external/catch/"
    "${PROJECT_SOURCE_DIR}/external/opencv/build/include"
    "${PROJECT_SOURCE_DIR}/external/glfw/include"
    "${PROJECT_SOURCE_DIR}/external/glew/include")

set(SOURCE_FILES
        TestMain.cpp
        TestMarkerDetector.cpp
        TestUtil.cpp)

add_executable(testrunner ${SOURCE_FILES})
target_link_libraries(testrunner yarrar ${OpenCV_LIBS})

add_test(
    NAME TestRunner
    COMMAND ${CMAKE_BINARY_DIR}/build/bin/testrunner
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

if(MSVC)
    include("${PROJECT_SOURCE_DIR}/cmake/windows/CopyDLLDependencies.cmake")
    copy_dll_dependencies(testrunner)
endif()
