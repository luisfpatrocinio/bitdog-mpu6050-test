/**
 * @file gyro.h
 * @brief Header file for MPU6050 accelerometer and gyroscope sensor interface.
 *
 * This file defines constants, declares global variables, and prototypes functions
 * for initializing and reading data from the MPU6050 sensor. It provides functions
 * for reading raw accelerometer data and calculating inclination angles.
 *
 * @author Luis Felipe Patrocinio (https://github.com/luisfpatrocinio)
 */
#ifndef GYRO_H
#define GYRO_H

// Hardware
#include "pico/stdlib.h"  ///< Pico SDK: Standard library functions.
#include "hardware/i2c.h" ///< Pico SDK: I2C hardware interface.
#include <math.h>         ///< Standard C: Mathematical functions.

// --- MPU6050 Configuration Constants ---

#define I2C_PORT i2c1                        ///< I2C port used for MPU6050 communication.
#define MPU6050_ADDR 0x68                    ///< I2C address of the MPU6050 sensor.
#define SDA_PIN 2                            ///< GPIO pin for I2C SDA line.
#define SCL_PIN 3                            ///< GPIO pin for I2C SCL line.
#define ACCEL_FS_SEL_2G_SENSITIVITY 16384.0f // LSB/g for ±2g range

// Dados do sensor
typedef struct
{
    int16_t raw_x, raw_y, raw_z;
    float g_x, g_y, g_z;
    float roll, pitch;
} MPU6050_data_t;

// --- Function Prototypes ---

/**
 * @brief Initializes the MPU6050 sensor.
 *
 * This function sends a reset command to the MPU6050's Power Management 1 register
 * to wake it up and prepare it for operation. It must be called before any other
 * MPU6050 functions.
 *
 * @note This function assumes I2C has been properly initialized.
 */
int initMPU6050();

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
 */
void updateAccelerometerData(MPU6050_data_t *data);

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
 */
void calculateInclinationAngles(MPU6050_data_t *data);

#endif // GYRO_H
