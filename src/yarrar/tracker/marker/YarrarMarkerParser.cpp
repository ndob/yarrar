#include "YarrarMarkerParser.h"
#include "Util.h"

#include <cassert>
#include <cstdint>

namespace {

using namespace yarrar;

const int FIELD_SIZE = 8;

int getId(const cv::Mat& field)
{
    // TODO: A real parser. This is only for testing.
    // Final id is fourth line as binary converted to integer.
    int id = 0;
    for(int i = 0; i < FIELD_SIZE; ++i)
    {
        if(field.at<uint8_t>(3, i) == 1)
        {
            id = id | (uint8_t) (1 << (7 - i));
        }
    }

    return id;
}

Rotation90 getZRotation(const cv::Mat& field)
{
    // The rectangle in upper left corner is used to
    // indicate the rotation.

    if(field.at<uchar>(1, 1) == 1) return Rotation90::DEG_0;
    else if(field.at<int8_t>(1, 6) == 1) return Rotation90::DEG_90;
    else if(field.at<int8_t>(6, 6) == 1) return Rotation90::DEG_180;
    else if(field.at<int8_t>(6, 1) == 1) return Rotation90::DEG_270;

    return Rotation90::DEG_0;
}

}

namespace yarrar {

MarkerValue YarrarMarkerParser::getData(const cv::Mat& image)
{
    assert(image.cols == image.rows && "Id parser expects square image");

    cv::Mat binary;
    cv::inRange(image, 0, 100, binary);

    // Works by dividing the image to 8x8 matrix (byte per line).
    // Then check if a cell is either black (1) or white (0).
    const int dimension = FIELD_SIZE;
    const int stepSize = image.cols / dimension;
    const int start = stepSize / 2;
    //DataField field;

    cv::Mat field = cv::Mat::zeros(dimension, dimension, CV_8S);

    for(int i = 0; i < dimension; ++i)
    {
        for(int j = 0; j < dimension; ++j)
        {
            const int col = start + i * stepSize;
            const int row = start + j * stepSize;
            const bool isBlack = binary.at<uint8_t>(col, row) > 0;
            field.at<int8_t>(i, j) = isBlack ? 1 : 0;
        }
    }

    // Get current rotation.
    Rotation90 rot = getZRotation(field);

    // Rotate the field, so that it's in default (DEG_0)
    // rotation. This way id parsing doesn't
    // have to care about rotations.
    switch(rot)
    {
        case Rotation90::DEG_90:
            util::rotate(field, field, Rotation90::DEG_270);
            break;
        case Rotation90::DEG_180:
            util::rotate(field, field, Rotation90::DEG_180);
            break;
        case Rotation90::DEG_270:
            util::rotate(field, field, Rotation90::DEG_90);
            break;
        case Rotation90::DEG_0:
            break;
    }

    return {
        true, // TODO: Validation
        getId(field),
        rot
    };
}

}
