#include "YarrarMarkerParser.h"

#include <array>
#include <cassert>

typedef std::array<std::array<bool, 8>, 8> DataField;

namespace {

using namespace yarrar;

int getId(const DataField& field)
{
    // TODO: A real parser. This is only for testing.
    // Final id is fourth line as binary converted to integer.
    unsigned char id = 0;
    for(size_t i = 0; i < field[3].size(); ++i)
    {
        if(field[3][i])
        {
            id = id | (unsigned char) (1 << (7 - i));
        }
    }

    return id;
}

Rotation90 getZRotation(const DataField& field)
{

    return Rotation90::DEG_0;
}


/*
Example print:
oooooooo
oooxoxxo
ooxooxoo
oxxxxooo
oxxoxxoo
oxoxxoxo
ooooxxoo
oooooooo
*/
void print(const DataField& field)
{
    std::cout << "\n\n\n\n";

    for(const auto& f : field)
    {
        for(const auto& f2 : f)
        {
            if(f2)
                std::cout << "x";
            else
                std::cout << "o";
        }
        std::cout << "\n";
    }
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
    const int dimension = 8;
    const int stepSize = image.cols / dimension;
    const int start = stepSize / 2;
    DataField field;

    for(int i = 0; i < dimension; ++i)
    {
        for(int j = 0; j < dimension; ++j)
        {
            const int col = start + i * stepSize;
            const int row = start + j * stepSize;
            const bool isBlack = binary.at<uchar>(col, row) > 0;
            field[i][j] = isBlack;
        }
    }

    return {
        getId(field),
        getZRotation(field)
    };
}

}
