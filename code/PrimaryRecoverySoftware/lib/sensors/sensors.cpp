#include <sensors.h>

#define SEA_LEVEL_PRESSURE 102400

Adafruit_BMP085 bmp;
Adafruit_MPU6050 mpu;

void init_mpu()
{
    debugln("MPU6050 test!");
    if (!mpu.begin())
    {
        debugln("Could not find a valid MPU6050 sensor, check wiring!");
        while (1)
        {
            // TODO: add beep to notify
        }
    }
    else
    {
        debugln("MPU6050 FOUND");
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
}

void init_bmp()
{
    debugln("BMP180 INITIALIZATION");
    if (!bmp.begin())
    {
        debugln("Could not find a valid BMP180 sensor, check wiring!");
        while (1)
        {
            // TODO: add beep to notify
        }
    }
    else
    {
        debugln("BMP180 FOUND");
    }
}

void init_bmp_mpu_sensors()
{
    init_bmp();
    init_mpu();
}

struct SensorReadings get_bmp_mpu_readings()
{
    struct SensorReadings sensorReadings = {0};
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    sensorReadings.ax = a.acceleration.x;
    sensorReadings.ay = a.acceleration.y;
    sensorReadings.az = a.acceleration.z;
    sensorReadings.gx = g.gyro.x;
    sensorReadings.gy = g.gyro.y;
    sensorReadings.gz = g.gyro.z;

    sensorReadings.altitude = bmp.readAltitude(SEA_LEVEL_PRESSURE);

    return sensorReadings;
}

float get_base_altitude()
{
    float altitude = 0;
    struct FilteredValues filtered_values = {0};
    struct SensorReadings sensorReadings = {0};

    for (int i = 0; i < 100; i++)
    {
        sensorReadings = get_bmp_mpu_readings();
        // TODO: very important to know the orientation of the altimeter
        filtered_values = kalmanUpdate(sensorReadings.altitude, sensorReadings.ay);
        altitude = altitude + filtered_values.displacement;
    }
    altitude = altitude / 100.0;
    debugf("Base Altitude is %.3f\n", altitude);
    return altitude;
}
