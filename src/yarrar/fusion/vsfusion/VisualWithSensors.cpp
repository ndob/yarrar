#include "VisualWithSensors.h"

namespace
{

using StateVector = yarrar::VSFusion::StateVector;
using StateMatrix = yarrar::VSFusion::StateMatrix;
using MeasurementVector = yarrar::VSFusion::MeasurementVector;
using MeasurementMatrix = yarrar::VSFusion::MeasurementMatrix;
using MeasurementUncertaintyMatrix = yarrar::VSFusion::MeasurementUncertaintyMatrix;

const StateVector xInitial = StateVector::Zero();
const StateVector uInitial = StateVector::Zero();
const StateMatrix pInitial = StateMatrix::Zero();
const MeasurementUncertaintyMatrix rInitial = MeasurementUncertaintyMatrix::Zero();

std::tuple<StateVector, StateMatrix> transition(const StateVector& state)
{
    StateVector newState = StateVector::Zero();
    StateMatrix jacobian = StateMatrix::Zero();
    return std::make_tuple(newState, jacobian);
}

std::tuple<MeasurementVector, MeasurementMatrix> measurement(const StateVector& state)
{
    MeasurementVector newMeasurement = MeasurementVector::Zero();
    newMeasurement[0] = state[0];
    newMeasurement[1] = state[1];

    MeasurementMatrix jacobian = MeasurementMatrix::Identity();

    return std::make_tuple(newMeasurement, jacobian);
}
}

namespace yarrar
{

VisualWithSensors::VisualWithSensors(const json11::Json& config)
    : SensorFusion(config)
    , m_kalman(transition, measurement, xInitial, pInitial, uInitial, rInitial)
{
}

void VisualWithSensors::getFusedPoses(const std::vector<std::reference_wrapper<const LockableData<Datapoint>>>& datapoints,
    const std::map<size_t, std::vector<Pose>>& poseLists,
    std::vector<Pose>& output)
{
    // datapoints:
    // 0. rotation-vector
    // 1. gyroscope

    // pose list contains poses from visual tracking
}
}
