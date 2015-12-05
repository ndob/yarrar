#include "yarrar/dataprovider/StaticImageDataProvider.h"
#include "yarrar/detector/MarkerDetector.h"
#include "yarrar/Pose.h"

#include "Util.h"

#include "catch.hpp"

namespace yarrar_test {

TEST_CASE("Pose is detected correctly", "[marker_detector]")
{
    using namespace yarrar;

    StaticImageDataProvider provider("data/img/marker.jpg");
    auto dim = provider.getDimensions();
    MarkerDetector detector(dim.width, dim.height);
    auto pose = detector.getPose(provider.getData());

    const double EPSILON = 0.001;

    REQUIRE(pose.valid);
    REQUIRE(pose.coordinateSystemId == 120);

    REQUIRE(almostEqual(pose.rotation.at<double>(0), 2.5418, EPSILON));
    REQUIRE(almostEqual(pose.rotation.at<double>(1), -0.2277, EPSILON));
    REQUIRE(almostEqual(pose.rotation.at<double>(2), 0.1797, EPSILON));

    REQUIRE(almostEqual(pose.translation.at<double>(0), 1.0704, EPSILON));
    REQUIRE(almostEqual(pose.translation.at<double>(1), -0.3123, EPSILON));
    REQUIRE(almostEqual(pose.translation.at<double>(2), 8.8115, EPSILON));

    REQUIRE(almostEqual(pose.camera.at<float>(0, 0), 686.2421f, static_cast<float>(EPSILON)));
    REQUIRE(almostEqual(pose.camera.at<float>(0, 1), 0.0f, static_cast<float>(EPSILON)));
    REQUIRE(almostEqual(pose.camera.at<float>(0, 2), 320.0f, static_cast<float>(EPSILON)));

    REQUIRE(almostEqual(pose.camera.at<float>(1, 0), 0.0f, static_cast<float>(EPSILON)));
    REQUIRE(almostEqual(pose.camera.at<float>(1, 1), 686.2421f, static_cast<float>(EPSILON)));
    REQUIRE(almostEqual(pose.camera.at<float>(1, 2), 426.5f, static_cast<float>(EPSILON)));

    REQUIRE(almostEqual(pose.camera.at<float>(2, 0), 0.0f, static_cast<float>(EPSILON)));
    REQUIRE(almostEqual(pose.camera.at<float>(2, 1), 0.0f, static_cast<float>(EPSILON)));
    REQUIRE(almostEqual(pose.camera.at<float>(2, 2), 1.0f, static_cast<float>(EPSILON)));
}

}