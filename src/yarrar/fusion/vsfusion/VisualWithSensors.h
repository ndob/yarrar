#pragma once

#include "PipelineStage.h"
#include "ExtendedKalmanFilter.h"

namespace yarrar
{
/*
 * Kalman filter configuration
 *
 * State (13 elements):
 * omega = angular velocity
 * q = quaternion orientation
 * t = position
 * v = velocity
 * [omega_x, omega_y, omega_z, q0, q1, q2, q3, t_x, t_y, t_z, v_x, v_y, v_z]
 *
 * Measurement (14 elements):
 * omega = angular velocity from gyroscope
 * qs = quaternion orientation from sensors
 * qt = quaternion orientation from visual tracking
 * t = position from visual tracking
 * [omega_x, omega_y, omega_z, qs0, qs1, qs2, qs3, qt0, qt1, qt2, qt3, t_x, t_y, t_z]
 */
using VSFusion = ExtendedKalmanFilter<13, 14>;

class VisualWithSensors : public SensorFusion
{
public:
    VisualWithSensors(const json11::Json& config);
    void getFusedPoses(const std::map<size_t, std::vector<Pose>>& poseLists,
        std::vector<Pose>& output) override;

private:
    VSFusion m_kalman;
};
}
