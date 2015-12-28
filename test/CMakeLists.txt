enable_testing()

include_directories(
    "${PROJECT_SOURCE_DIR}/src" 
    "${PROJECT_SOURCE_DIR}/src/yarrar"
    "${PROJECT_SOURCE_DIR}/external/catch/"
    "${PROJECT_SOURCE_DIR}/external/opencv/build/include"
    "${PROJECT_SOURCE_DIR}/external/glfw/include"
    "${PROJECT_SOURCE_DIR}/external/glew/include")

set(SOURCE_FILES TestMain.cpp TestMarkerDetector.cpp)

add_executable(testrunner ${SOURCE_FILES})
target_link_libraries(testrunner yarrar ${OpenCV_LIBS})

add_test(
    NAME TestMarkerDetector 
    COMMAND ${CMAKE_BINARY_DIR}/build/bin/testrunner
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

if(MSVC)
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/external/glew/bin/Release/x64/*.dll" GLEW_DLL_FILES)
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/external/glfw/lib-vc2015/*.dll" GLFW_DLL_FILES)
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/external/opencv/build/x64/vc14/bin" OPENCV_DLL_FILES)
    file(TO_NATIVE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>" MSVC_BINARY_OUTPUT_PATH)

    add_custom_command(TARGET testrunner POST_BUILD
        COMMAND copy "${GLEW_DLL_FILES}" "${MSVC_BINARY_OUTPUT_PATH}")
    add_custom_command(TARGET testrunner POST_BUILD
        COMMAND copy "${GLFW_DLL_FILES}" "${MSVC_BINARY_OUTPUT_PATH}")
    add_custom_command(TARGET testrunner POST_BUILD
        COMMAND copy "${OPENCV_DLL_FILES}" "${MSVC_BINARY_OUTPUT_PATH}")
endif()
