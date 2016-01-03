#pragma once

#include "MarkerParser.h"

namespace yarrar {

class YarrarMarkerParser: public MarkerParser
{
public:
    virtual MarkerValue getData(const cv::Mat& image) override;

};

}
