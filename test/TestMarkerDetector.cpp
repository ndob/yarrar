#include "yarrar/dataprovider/StaticImageDataProvider.h"
#include "yarrar/tracker/marker/MarkerTracker.h"
#include "yarrar/tracker/marker/YarrarMarkerParser.h"

#include "yarrar/Util.h"

#include "catch.hpp"

namespace yarrar_test {

TEST_CASE("Pose is detected correctly", "[marker_tracker]")
{
    using namespace yarrar;

    const int TRACKING_RESOLUTION = 320;

    StaticImageDataProvider provider("data/img/marker.jpg");
    auto dim = provider.getDimensions();
    MarkerTracker<YarrarMarkerParser> detector(getScaledDownResolution(dim.width, dim.height, TRACKING_RESOLUTION));
    auto pose = detector.getPose(provider.getData());

    const double EPSILON = 0.01;

    REQUIRE(pose.valid);
    REQUIRE(pose.coordinateSystemId == 120);

    REQUIRE(pose.rotation.at<double>(0) == Approx(2.5418).epsilon(EPSILON));
    REQUIRE(pose.rotation.at<double>(1) == Approx(-0.2277).epsilon(EPSILON));
    REQUIRE(pose.rotation.at<double>(2) == Approx(0.2011).epsilon(EPSILON));

    REQUIRE(pose.translation.at<double>(0) == Approx(1.0704).epsilon(EPSILON));
    REQUIRE(pose.translation.at<double>(1) == Approx(-0.3123).epsilon(EPSILON));
    REQUIRE(pose.translation.at<double>(2) == Approx(8.8115).epsilon(EPSILON));

    float ratio1 = pose.camera.at<float>(0, 0) / pose.camera.at<float>(0, 2);
    float ratio2 = pose.camera.at<float>(1, 1) / pose.camera.at<float>(1, 2);
    REQUIRE(ratio1 == Approx(2.14f).epsilon(static_cast<float>(EPSILON)));
    REQUIRE(ratio2 == Approx(1.61f).epsilon(static_cast<float>(EPSILON)));
    REQUIRE(pose.camera.at<float>(0, 1) == Approx(0.0f).epsilon(EPSILON));
    REQUIRE(pose.camera.at<float>(1, 0) == Approx(0.0f).epsilon(EPSILON));
    REQUIRE(pose.camera.at<float>(2, 0) == Approx(0.0f).epsilon(EPSILON));
    REQUIRE(pose.camera.at<float>(2, 1) == Approx(0.0f).epsilon(EPSILON));
    REQUIRE(pose.camera.at<float>(2, 2) == Approx(1.0f).epsilon(EPSILON));
}

}