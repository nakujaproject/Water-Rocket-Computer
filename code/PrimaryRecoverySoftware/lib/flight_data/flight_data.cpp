#include <flight_data.h>

static float previousAltitude = 0;


struct FlightData formatFlightData(SensorReadings sensorReadings, FilteredValues filteredValues)
{
    struct FlightData flightData = {0};
    flightData.altitude = sensorReadings.altitude;
    flightData.ax = sensorReadings.ax;
    flightData.ay = sensorReadings.ay;
    flightData.az = sensorReadings.az;
    flightData.gx = sensorReadings.gx;
    flightData.gy = sensorReadings.gy;
    flightData.gz = sensorReadings.gz;
    flightData.filtered_s = filteredValues.displacement;
    flightData.filtered_a = filteredValues.acceleration;
    flightData.filtered_v = filteredValues.velocity;
    return flightData;
}

struct FlightData readFlightData()
{
    struct FlightData flightData = {0};
    struct SensorReadings sensorReadings = {0};
    struct FilteredValues filteredValues = {0};

    sensorReadings = get_bmp_mpu_readings();

    // TODO: very important to know the orientation of the altimeter
    filteredValues = kalmanUpdate(sensorReadings.altitude, sensorReadings.ay);

    flightData = formatFlightData(sensorReadings, filteredValues);

    float currentAltitude = flightData.filtered_s;

    // Update state
    state = checkState(currentAltitude, previousAltitude, flightData.filtered_v, flightData.filtered_a, state);

    flightData.state = state;
    flightData.timeStamp = millis();
    previousAltitude = currentAltitude;

    // get gps readings
    GPSReadings gpsReadings = get_gps_readings();
    flightData.latitude = gpsReadings.latitude;
    flightData.longitude = gpsReadings.longitude;

    return flightData;
}
