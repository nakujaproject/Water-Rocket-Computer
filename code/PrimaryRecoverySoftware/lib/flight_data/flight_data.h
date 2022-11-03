#ifndef FLIGHTDATA_H
#define FLIGHTDATA_H

#include <defs.h>
#include <sensors.h>
#include <kalman_filter.h>
#include <states.h>
#include <gps.h>

// This struct is used to save all our datapoints.
// It includes rocket altitude, accelerations in the x, y and z directions
// Gryroscope values in the x, y and z direcion
// filtered altitude, velocity and acceleration
// GPS longitude, laltitude and altitude and state
struct FlightData
{
    uint64_t timeStamp;
    float altitude;
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
    float filtered_s;
    float filtered_v;
    float filtered_a;
    int state;
    float latitude;
    float longitude;
};

struct FlightData readFlightData();
struct FlightData formatFlightData(SensorReadings sensorReadings, FilteredValues filteredValues);

#endif // FLIGHTDATA_H