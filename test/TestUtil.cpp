#include "yarrar/Util.h"

#include <catch.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

namespace
{

enum Color
{
    WHITE,
    RED,
    GREEN,
    BLUE
};
typedef std::array<std::array<Color, 2>, 2> Grid;

void validateGrid(const cv::Mat& image, const Grid& colors)
{
    for(size_t i = 0; i < colors.size(); ++i)
    {
        for(size_t j = 0; j < colors[i].size(); ++j)
        {
            auto pixel = image.at<cv::Vec3b>(i, j);
            switch(colors[i][j])
            {
                case WHITE:
                    REQUIRE(pixel[0] == 255);
                    REQUIRE(pixel[1] == 255);
                    REQUIRE(pixel[2] == 255);
                    break;
                case RED:
                    REQUIRE(pixel[0] == 0);
                    REQUIRE(pixel[1] == 0);
                    REQUIRE(pixel[2] == 255);
                    break;
                case GREEN:
                    REQUIRE(pixel[0] == 0);
                    REQUIRE(pixel[1] == 255);
                    REQUIRE(pixel[2] == 0);
                    break;
                case BLUE:
                    REQUIRE(pixel[0] == 255);
                    REQUIRE(pixel[1] == 0);
                    REQUIRE(pixel[2] == 0);
                    break;
            }
        }
    }
}
}

namespace yarrar_test
{

TEST_CASE("Image is rotated correctly", "[util]")
{
    cv::Mat src, dst;
    src = cv::imread("test/fixture/img/util_rotate_test.png", cv::IMREAD_COLOR);

    SECTION("0 degrees")
    {
        yarrar::util::rotate(src, dst, yarrar::Rotation90::DEG_0);

        Grid grid{ { { { RED, GREEN } },
            { { BLUE, WHITE } } } };

        validateGrid(dst, grid);
    }
    SECTION("90 degrees")
    {
        yarrar::util::rotate(src, dst, yarrar::Rotation90::DEG_90);
        Grid grid{ { { { BLUE, RED } },
            { { WHITE, GREEN } } } };

        validateGrid(dst, grid);
    }
    SECTION("180 degrees")
    {
        yarrar::util::rotate(src, dst, yarrar::Rotation90::DEG_180);
        Grid grid{ { { { WHITE, BLUE } },
            { { GREEN, RED } } } };

        validateGrid(dst, grid);
    }
    SECTION("270 degrees")
    {
        yarrar::util::rotate(src, dst, yarrar::Rotation90::DEG_270);
        Grid grid{ { { { GREEN, WHITE } },
            { { RED, BLUE } } } };

        validateGrid(dst, grid);
    }
}
}
