#include "yarrar/ExtendedKalmanFilter.h"

#include <catch.hpp>
#include <cmath>
#include <tuple>
#include <iostream>
#include <fstream>

namespace yarrar_test
{
// State: [x, y, distance_diff_between_measurements, angle_diff_between_measurements, moved_angle]
using CircularMotion = yarrar::ExtendedKalmanFilter<5, 2>;

float distanceBetween(float x0, float y0, float x1, float y1)
{
    return std::sqrt(std::pow(x1 - x0, 2) + std::pow(y1 - y0, 2));
}

// Clamps between [-pi, pi]
float clampAngle(float angle)
{
    while(angle < 0)
    {
        angle += M_PI * 2;
    }
    return static_cast<float>(fmod((angle + M_PI), (M_PI * 2)) - M_PI);
}

std::tuple<CircularMotion::StateVector, CircularMotion::StateMatrix> transition(const CircularMotion::StateVector& state)
{
    float distanceDiff = state[2];
    float angleDiff = state[3];
    float theta = clampAngle(state[4] + angleDiff);

    float x = state[0] + std::cos(theta) * distanceDiff;
    float y = state[1] + std::sin(theta) * distanceDiff;

    float moved_angle = std::atan2(y - state[1], x - state[0]);
    float new_angle_diff = clampAngle(moved_angle - state[4]);
    float new_distance_diff = distanceBetween(x, y, state[0], state[1]);

    CircularMotion::StateVector vec;
    vec << x, y, new_distance_diff, new_angle_diff, theta;

    CircularMotion::StateMatrix jacobian;
    jacobian << 1.f, 0.f, std::cos(theta), 0.f, -1.f * std::sin(theta) * distanceDiff,
        0.f, 1.f, std::sin(theta), 0.f, std::cos(theta) * distanceDiff,
        0.f, 0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f, 1.f;

    return std::make_tuple(vec, jacobian);
}

std::tuple<CircularMotion::MeasurementVector, CircularMotion::MeasurementMatrix> measurement(const CircularMotion::StateVector& state)
{
    CircularMotion::MeasurementVector measurement = CircularMotion::MeasurementVector::Zero();
    measurement[0] = state[0];
    measurement[1] = state[1];

    CircularMotion::MeasurementMatrix jacobian = CircularMotion::MeasurementMatrix::Identity();

    return std::make_tuple(measurement, jacobian);
}

TEST_CASE("Measures circular motion correctly", "[kalman_filter]")
{
    // Read measurement values.
    std::vector<CircularMotion::MeasurementVector> measurements;
    measurements.reserve(1000);
    {
        std::ifstream file("test/fixture/kalman_circular_measurements.txt");
        while(file.good())
        {
            CircularMotion::MeasurementVector coordinate;
            file >> coordinate[0] >> coordinate[1];
            measurements.push_back(coordinate);
        }
    }

    // Read control values.
    std::vector<std::pair<float, float>> controlValues;
    controlValues.reserve(1000);
    {
        std::ifstream file("test/fixture/kalman_control_values.txt");
        while(file.good())
        {
            std::pair<float, float> coordinate;
            file >> coordinate.first >> coordinate.second;
            controlValues.push_back(coordinate);
        }
    }

    REQUIRE(measurements.size() == 1000);
    REQUIRE(controlValues.size() == 1000);

    // Initialize Kalman filter.
    CircularMotion::StateVector xInitial = CircularMotion::StateVector::Zero();
    float moved_angle = (float) atan2(measurements[2][1] - measurements[1][1], measurements[2][0] - measurements[1][0]);
    float angle_diff = moved_angle - (float) atan2(measurements[1][1] - measurements[0][1], measurements[1][0] - measurements[0][0]);
    float distance_diff = distanceBetween(measurements[1][0], measurements[1][1], measurements[0][0], measurements[0][1]);

    xInitial << measurements[2][0], measurements[2][1], distance_diff, angle_diff, moved_angle;

    CircularMotion::StateMatrix pInitial;
    float init = 999.f;
    pInitial << init, 0.f, 0.f, 0.f, 0.f,
        0.f, init, 0.f, 0.f, 0.f,
        0.f, 0.f, init, 0.f, 0.f,
        0.f, 0.f, 0.f, init, 0.f,
        0.f, 0.f, 0.f, 0.f, init;

    CircularMotion::StateVector uInitial = CircularMotion::StateVector::Zero();

    CircularMotion::MeasurementUncertaintyMatrix RInitial;
    RInitial << 0.5f, 0.f,
        0.f, 0.5f;

    CircularMotion filter(transition, measurement, xInitial, pInitial, uInitial, RInitial);

    const float EPSILON = 0.1f;
    int32_t localizedAmount = 0;

    for(size_t i = 2; i < measurements.size(); ++i)
    {
        filter.update(measurements[i]);

        auto state = filter.state();
        if(std::abs(controlValues[i].first - state[0]) < EPSILON && std::abs(controlValues[i].second - state[1]) < EPSILON)
        {
            localizedAmount++;
        }
    }

    REQUIRE(localizedAmount == 945);
}
}
