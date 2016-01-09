#pragma once

#include "Types.h"

#include <opencv2/core/types.hpp>
#include <json11.hpp>
#include <algorithm>
#include <iterator>
#include <cstdio>

namespace yarrar
{
namespace util
{

template <typename Container, typename Value>
bool contains(const Container& c, const Value& v)
{
    return std::find(std::begin(c), std::end(c), v) != std::end(c);
}

template <typename... Args>
std::string format(const std::string& format, Args... args)
{
    int neededSize = snprintf(nullptr, 0, format.c_str(), args...);

    // If there was an error return the original string.
    if(neededSize <= 0)
        return format;

    // Accommodate \0
    neededSize += 1;

    std::string buf;
    buf.resize(static_cast<size_t>(neededSize));
    snprintf(&buf.front(), static_cast<size_t>(neededSize), format.c_str(), args...);
    return buf;
}

cv::Size getScaledDownResolution(const int width,
    const int height,
    const int preferredWidth);

void rotate(const cv::Mat& src, cv::Mat& dst, const yarrar::Rotation90& rotation);

json11::Json loadJson(const std::string& filePath);
}
}
