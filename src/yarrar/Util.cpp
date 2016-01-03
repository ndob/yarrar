#include "Util.h"

#include <opencv2/core.hpp>

namespace yarrar {

cv::Size getScaledDownResolution(const int width,
                                 const int height,
                                 const int preferredWidth)
{
    if(width <= preferredWidth)
    {
        std::pair<int, int> (width, height);
    }
    float aspect = static_cast<float> (width) / static_cast<float> (height);

    return cv::Size(
            preferredWidth,
            static_cast<int> (std::floor(preferredWidth / aspect))
    );
}

void rotate(const cv::Mat& src, cv::Mat& dst, const yarrar::Rotation90& rotation)
{
    using yarrar::Rotation90;

    dst.create(src.rows, src.cols, src.type());

    // Last parameter of cv::flip():
    // 0  -> flip around x-axis
    // 1  -> flip around y-axis
    // -1 -> flip around both axes

    switch(rotation)
    {
        case Rotation90::DEG_90:
            cv::flip(src.t(), dst, 1);
            break;
        case Rotation90::DEG_180:
            cv::flip(src, dst, -1);
            break;
        case Rotation90::DEG_270:
            cv::flip(src.t(), dst, 0);
            break;
        case Rotation90::DEG_0:
            if(src.data != dst.data) src.copyTo(dst);
            break;
    }
}

}
