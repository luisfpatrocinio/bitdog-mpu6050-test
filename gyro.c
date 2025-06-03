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

/**
 * @brief Initialize the MPU6050 sensor.
 */
void initMPU6050()
{
    uint8_t setup_data[2];

    // Wake up MPU6050 - Power Management 1 register
    setup_data[0] = 0x6B; // PWR_MGMT_1 register
    setup_data[1] = 0x00; // Clear sleep bit

    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, setup_data, 2, false);
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
 * @brief Reads raw gyroscope data from the MPU6050 sensor.
 *
 * This function reads 6 bytes from the MPU6050, starting from the GYRO_XOUT_H register (0x43),
 * which contain the high and low bytes for X, Y, and Z-axis angular velocity.
 * The raw 16-bit values are then assembled and stored in the provided MPU6050_data_t structure.
 *
 * @param data Pointer to an MPU6050_data_t structure to store the raw gyroscope data.
 * @note The raw values are in the range of -32768 to 32767.
 */
void updateGyroscopeData(MPU6050_data_t *data)
{
    uint8_t buffer[6];
    uint8_t reg = 0x43; // Gyroscope data register address (GYRO_XOUT_H)

    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buffer, 6, false);

    data->g_x = (buffer[0] << 8) | buffer[1];
    data->g_y = (buffer[2] << 8) | buffer[3];
    data->g_z = (buffer[4] << 8) | buffer[5];
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
    data->g_x = data->raw_x / ACCEL_FS_SEL_2G_SENSITIVITY;
    data->g_y = data->raw_y / ACCEL_FS_SEL_2G_SENSITIVITY;
    data->g_z = data->raw_z / ACCEL_FS_SEL_2G_SENSITIVITY;

    // Calculate Roll (rotation around X-axis)
    data->roll = atan2(data->g_y, data->g_z) * (180.0 / M_PI);

    // Calculate Pitch (rotation around Y-axis)
    data->pitch = atan2(-data->g_x, sqrt(data->g_y * data->g_y + data->g_z * data->g_z)) * (180.0 / M_PI);
}
