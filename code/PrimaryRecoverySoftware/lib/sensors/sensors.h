#ifndef SENSORS_H
#define SENSORS_H

#include <defs.h>
#include <kalman_filter.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// function to initialize mpu
void init_mpu();

// function to initialize bmp
void init_bmp();

// function to initialize bmp and mpu
void init_bmp_mpu_sensors();

// Get BMP and MPU sensor readings
struct SensorReadings get_bmp_mpu_readings();

// get_base_altitude Finds the average of the current altitude from 100 readings
float get_base_altitude();

// struct containing measurements from bmp and mpu
struct SensorReadings
{
    float altitude;
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
};

#endif // SENSORS_H