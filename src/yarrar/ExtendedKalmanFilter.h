#pragma once

#include <Eigen/Core>
#include <Eigen/LU>

#include <functional>
#include <tuple>

namespace yarrar
{

template <
    int StateDimensions,
    int MeasurementDimensions,
    typename ValueType = float>
class ExtendedKalmanFilter
{
public:
    using StateVector = Eigen::Matrix<ValueType, StateDimensions, 1>;
    using StateMatrix = Eigen::Matrix<ValueType, StateDimensions, StateDimensions>;
    using TransitionFunction = std::function<std::tuple<StateVector, StateMatrix>(StateVector)>;

    using MeasurementVector = Eigen::Matrix<ValueType, MeasurementDimensions, 1>;
    using MeasurementMatrix = Eigen::Matrix<ValueType, MeasurementDimensions, StateDimensions>;
    using MeasurementUncertaintyMatrix = Eigen::Matrix<ValueType, MeasurementDimensions, MeasurementDimensions>;
    using MeasurementFunction = std::function<std::tuple<MeasurementVector, MeasurementMatrix>(StateVector)>;

    ExtendedKalmanFilter(const TransitionFunction& transitionFunc,
        const MeasurementFunction& measurement,
        const StateVector& xInitial = StateVector::Zero(),
        const StateMatrix& PInitial = StateMatrix::Zero(),
        const StateVector& uInitial = StateVector::Zero(),
        const MeasurementUncertaintyMatrix& RInitial = MeasurementUncertaintyMatrix::Zero())
        : m_transitionFunc(transitionFunc)
        , m_measurementFunc(measurement)
        , x(xInitial)
        , P(PInitial)
        , u(uInitial)
        , R(RInitial)
        , G(StateMatrix::Zero())
        , H(MeasurementMatrix::Zero())
        , I(StateMatrix::Identity())
    {
    }

    void update(const MeasurementVector& measurement)
    {
        // Predict (a priori estimate)
        std::tie(x, G) = m_transitionFunc(x);
        P = G * P * G.transpose();

        // Measurement (a posteriori estimate)
        // MeasurementVector, MeasurementMatrix
        MeasurementVector newX;
        std::tie(newX, H) = m_measurementFunc(x);
        auto y = measurement - newX;
        auto S = H * P * H.transpose() + R;
        auto K = P * H.transpose() * S.inverse();
        x = x + (K * y);
        P = (I - (K * H)) * P;
    }

    const StateVector& state()
    {
        return x;
    }

private:
    TransitionFunction m_transitionFunc;
    MeasurementFunction m_measurementFunc;
    StateVector x;
    StateMatrix P;
    StateVector u;
    MeasurementUncertaintyMatrix R;

    StateMatrix G;
    MeasurementMatrix H;
    StateMatrix I;
};
}
