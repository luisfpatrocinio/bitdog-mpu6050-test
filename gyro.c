#include "gyro.h"

// Define M_PI if not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// MPU6050 Variables
int16_t ax, ay, az;
float ax_g, ay_g, az_g;
float roll, pitch;

void initMPU6050()
{
    uint8_t reset[2] = {0x6B, 0x00}; // Power Management 1 register address and reset value
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, reset, 2, false);
}

void updateAccelerometerData(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t buffer[6];
    // Accelerometer data register address (ACCEL_XOUT_H)
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, (uint8_t[]){0x3B}, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buffer, 6, false);

    *ax = (buffer[0] << 8) | buffer[1];
    *ay = (buffer[2] << 8) | buffer[3];
    *az = (buffer[4] << 8) | buffer[5];
}

void calculateInclinationAngles(float *roll, float *pitch)
{
    // Convert raw accelerometer values to g's
    ax_g = ax / 16384.0;
    ay_g = ay / 16384.0;
    az_g = az / 16384.0;

    // Calculate Roll (rotation around X-axis)
    *roll = atan2(ay_g, az_g) * (180.0 / M_PI);

    // Calculate Pitch (rotation around Y-axis)
    *pitch = atan2(-ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * (180.0 / M_PI);
}
