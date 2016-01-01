#include "Util.h"

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

}
