project(json11)

cmake_minimum_required(VERSION 2.8)

add_definitions(
    -std=c++11
    -fno-rtti
    -fno-exceptions
    -Wall
    -Wextra
    -Werror)

set(json11_SRCS json11.cpp)

add_library(json11 STATIC ${json11_SRCS})
