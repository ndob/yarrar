#pragma once

#include <algorithm>
#include <iterator>
#include <opencv2/core/types.hpp>

namespace yarrar {

struct Vector3
{
    float x;
    float y;
    float z;
};

template<typename Container, typename Value>
bool contains(const Container& c, const Value& v) 
{
    return std::find(std::begin(c), std::end(c), v) != std::end(c);
}


cv::Size getScaledDownResolution(const int width,
                                 const int height,
                                 const int preferredWidth);

}
