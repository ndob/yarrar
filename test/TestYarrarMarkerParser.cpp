#include "yarrar/tracker/marker/YarrarMarkerParser.h"
#include "yarrar/Types.h"
#include "yarrar/Util.h"

#include <catch.hpp>

namespace yarrar_test {

TEST_CASE("Id and rotation is parsed correctly", "[yarrar_marker_parser]")
{
    using namespace yarrar;

    cv::Mat image = cv::imread("data/img/yarrar_marker_datafield.png", cv::IMREAD_GRAYSCALE);
    cv::Mat rotated;
    YarrarMarkerParser parser;

    SECTION("No rotation")
    {
        MarkerValue value = parser.getData(image);
        REQUIRE(value.id == 30);
        REQUIRE(value.zRotation == Rotation90::DEG_0);
    }

    SECTION("90 degrees rotation")
    {
        rotate(image, rotated, Rotation90::DEG_90);
        MarkerValue value = parser.getData(rotated);
        REQUIRE(value.id == 30);
        REQUIRE(value.zRotation == Rotation90::DEG_90);
    }

    SECTION("180 degrees rotation")
    {
        rotate(image, rotated, Rotation90::DEG_180);
        MarkerValue value = parser.getData(rotated);
        REQUIRE(value.id == 30);
        REQUIRE(value.zRotation == Rotation90::DEG_180);
    }

    SECTION("270 degrees rotation")
    {
        rotate(image, rotated, Rotation90::DEG_270);
        MarkerValue value = parser.getData(rotated);
        REQUIRE(value.id == 30);
        REQUIRE(value.zRotation == Rotation90::DEG_270);
    }
}

}