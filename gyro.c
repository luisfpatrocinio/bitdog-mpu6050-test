/**
 * @file gyro.c
 * @brief Implementation file for MPU6050 accelerometer and gyroscope sensor interface.
 *
 * This file implements the functions declared in gyro.h for initializing
 * and reading data from the MPU6050 sensor. It includes the implementation
 * of functions for reading raw accelerometer data and calculating inclination angles.
 *
 * @author Luis Felipe Patrocinio (https://github.com/luisfpatrocinio)
 */
#include "gyro.h"

// Define M_PI if not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- Global Variable Definitions ---

int16_t ax;  ///< Raw X-axis accelerometer data.
int16_t ay;  ///< Raw Y-axis accelerometer data.
int16_t az;  ///< Raw Z-axis accelerometer data.
float ax_g;  ///< X-axis acceleration in g's.
float ay_g;  ///< Y-axis acceleration in g's.
float az_g;  ///< Z-axis acceleration in g's.
float roll;  ///< Calculated roll angle in degrees.
float pitch; ///< Calculated pitch angle in degrees.

/**
 * @brief Initialize the MPU6050 sensor.
 * @return 0 if initialization is successful, non-zero otherwise.
 */
int initMPU6050()
{
    uint8_t reset[2] = {0x6B, 0x00}; // Power Management 1 register address and reset value
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, reset, 2, false);
    return 0;
}

/**
 * @brief Update the accelerometer data.
 * @param data Pointer to store the accelerometer data.
 */
void updateAccelerometerData(MPU6050_data_t *data)
{
    uint8_t buffer[6];
    // Accelerometer data register address (ACCEL_XOUT_H)
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, (uint8_t[]){0x3B}, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buffer, 6, false);

    data->raw_x = (buffer[0] << 8) | buffer[1];
    data->raw_y = (buffer[2] << 8) | buffer[3];
    data->raw_z = (buffer[4] << 8) | buffer[5];
}

/**
 * @brief Calculates the roll and pitch angles from accelerometer data.
 *
 * This function converts the raw accelerometer readings into acceleration in g's
 * and then uses these values to compute the roll and pitch angles, which represent
 * the inclination of the sensor.
 *
 * @param roll Pointer to store the calculated roll angle in degrees.
 * @param pitch Pointer to store the calculated pitch angle in degrees.
 * @note Roll is rotation around X-axis, Pitch is rotation around Y-axis.
 * @note The angles are calculated using atan2 and are in the range of -180 to 180 degrees.
 * @note The conversion factor ACCEL_FS_SEL_2G_SENSITIVITY (16384.0) is specific to the MPU6050's ±2g range.
 */
void calculateInclinationAngles(MPU6050_data_t *data)
{
    // Convert raw accelerometer values to g's
    ax_g = data->raw_x / ACCEL_FS_SEL_2G_SENSITIVITY;
    ay_g = data->raw_y / ACCEL_FS_SEL_2G_SENSITIVITY;
    az_g = data->raw_z / ACCEL_FS_SEL_2G_SENSITIVITY;

    // Calculate Roll (rotation around X-axis)
    data->roll = atan2(ay_g, az_g) * (180.0 / M_PI);

    // Calculate Pitch (rotation around Y-axis)
    data->pitch = atan2(-ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * (180.0 / M_PI);
}
