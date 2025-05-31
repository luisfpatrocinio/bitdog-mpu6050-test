#ifndef GYRO_H
#define GYRO_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <math.h>

// MPU6050 Definitions
#define I2C_PORT i2c1
#define MPU6050_ADDR 0x68
#define SDA_PIN 2
#define SCL_PIN 3

// MPU6050 Variables
extern int16_t ax, ay, az;
extern float ax_g, ay_g, az_g;
extern float roll, pitch;

/**
 * @brief Initializes the MPU6050 sensor.
 *
 * This function sends a reset command to the MPU6050's Power Management 1 register
 * to wake it up and prepare it for operation.
 */
void initMPU6050();

/**
 * @brief Reads raw accelerometer data from the MPU6050 sensor and updates the provided variables.
 *
 * This function reads 6 bytes from the MPU6050, starting from the ACCEL_XOUT_H register (0x3B),
 * which contain the high and low bytes for X, Y, and Z-axis acceleration.
 * The raw 16-bit values are then assembled and stored in the provided pointers.
 *
 * @param ax Pointer to an int16_t variable where the raw X-axis acceleration data will be stored.
 * @param ay Pointer to an int16_t variable where the raw Y-axis acceleration data will be stored.
 * @param az Pointer to an int16_t variable where the raw Z-axis acceleration data will be stored.
 */
void updateAccelerometerData(int16_t *ax, int16_t *ay, int16_t *az);

/**
 * @brief Calculates the roll and pitch angles from accelerometer data.
 *
 * This function converts the raw accelerometer readings (ax, ay, az) into
 * acceleration in g's and then uses these values to compute the roll and
 * pitch angles, which represent the inclination of the sensor.
 *
 * @param roll Pointer to a float variable where the calculated roll angle (in degrees) will be stored.
 * @param pitch Pointer to a float variable where the calculated pitch angle (in degrees) will be stored.
 */
void calculateInclinationAngles(float *roll, float *pitch);

#endif // GYRO_H
