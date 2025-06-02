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
 * @brief Initializes the MPU6050 sensor.
 *
 * This function sends a reset command to the MPU6050's Power Management 1 register
 * to wake it up and prepare it for operation. The reset value 0x00 clears the sleep
 * bit, enabling the sensor.
 *
 * @note This function assumes I2C has been properly initialized.
 * @note The Power Management 1 register (0x6B) is used to control the sensor's power mode.
 */
void initMPU6050()
{
    uint8_t reset[2] = {0x6B, 0x00}; // Power Management 1 register address and reset value
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, reset, 2, false);
}

/**
 * @brief Reads raw accelerometer data from the MPU6050 sensor.
 *
 * This function reads 6 bytes from the MPU6050, starting from the ACCEL_XOUT_H register (0x3B),
 * which contain the high and low bytes for X, Y, and Z-axis acceleration.
 * The raw 16-bit values are then assembled and stored in the provided pointers.
 *
 * @param ax Pointer to store the raw X-axis acceleration data.
 * @param ay Pointer to store the raw Y-axis acceleration data.
 * @param az Pointer to store the raw Z-axis acceleration data.
 * @note The raw values are in the range of -32768 to 32767.
 * @note The accelerometer data is read in a single I2C transaction for efficiency.
 */
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
 * @note The conversion factor 16384.0 is specific to the MPU6050's ±2g range.
 */
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
