#ifndef GPS_H
#define GPS_H
#include <defs.h>
#include <TinyGPS++.h>

// GPSReadings contains the gps informations that is
// latitude, longitude, speed, number of satellites and altitude
struct GPSReadings
{
    float latitude;
    float longitude;
};

struct GPSReadings get_gps_readings();
void init_gps();

#endif // GPS_H