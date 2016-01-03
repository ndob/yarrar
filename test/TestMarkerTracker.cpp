#include "yarrar/dataprovider/StaticImageDataProvider.h"
#include "yarrar/tracker/marker/MarkerTracker.h"

#include "yarrar/Util.h"

#include <json11.hpp>
#include <catch.hpp>

namespace yarrar_test {

TEST_CASE("Pose is detected correctly", "[marker_tracker]")
{
    using namespace yarrar;
    using namespace json11;

    Json staticImageConf = Json::object{
        { "image_path", "data/img/marker.jpg" }
    };
    StaticImageDataProvider provider(staticImageConf);

    auto dim = provider.getDimensions();

    Json markerTrackerConf = Json::object{
        { "parser", "yarrar_parser" },
        { "tracking_resolution_width", 320 }
    };
    MarkerTracker tracker(dim.width, dim.height, markerTrackerConf);

    std::vector<Pose> poses;
    tracker.getPoses(provider.getData(), poses);
    REQUIRE(poses.size() == 1);

    auto pose = poses.at(0);
    const double EPSILON = 0.01;

    REQUIRE(pose.coordinateSystemId == 30);

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