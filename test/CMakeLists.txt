enable_testing()

include_directories(
    "${PROJECT_SOURCE_DIR}/src" 
    "${PROJECT_SOURCE_DIR}/src/yarrar"
    "${PROJECT_SOURCE_DIR}/external/catch/"
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES TestMain.cpp TestMarkerDetector.cpp)

add_executable(testrunner ${SOURCE_FILES})
find_package(OpenCV REQUIRED)
target_link_libraries(testrunner libyarrar ${OpenCV_LIBS})

add_test(
    NAME TestMarkerDetector 
    COMMAND ${CMAKE_BINARY_DIR}/build/bin/testrunner
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
