#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <BasicLinearAlgebra.h>
#include <defs.h>

// struct containing filtered values from the kalman filter
struct FilteredValues
{
    float displacement;
    float velocity;
    float acceleration;
};

// kalmanUpdate This filteres our altitude and acceleration values
struct FilteredValues kalmanUpdate(float altitude, float acceleration);

#endif // KALMANFILTER_H