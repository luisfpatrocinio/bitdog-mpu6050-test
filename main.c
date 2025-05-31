#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <math.h>
#include <stdio.h>

// Patrolibs
#include "led.h"

// Definições de GPIOs e endereço do MPU6050
#define I2C_PORT i2c1
#define MPU6050_ADDR 0x68
#define SDA_PIN 2
#define SCL_PIN 3

// Definições do MPU6050
int16_t ax, ay, az;
float ax_g, ay_g, az_g;
float roll, pitch;

/**
 * @brief Initializes the MPU6050 sensor.
 *
 * This function sends a reset command to the MPU6050's Power Management 1 register
 * to wake it up and prepare it for operation.
 */
void initMPU6050()
{
    uint8_t reset[2] = {0x6B, 0x00}; // Power Management 1 register address and reset value
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, reset, 2, false);
}

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
 * This function converts the raw accelerometer readings (ax, ay, az) into
 * acceleration in g's and then uses these values to compute the roll and
 * pitch angles, which represent the inclination of the sensor.
 *
 * @param roll Pointer to a float variable where the calculated roll angle (in degrees) will be stored.
 * @param pitch Pointer to a float variable where the calculated pitch angle (in degrees) will be stored.
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

int main()
{
    sleep_ms(1000); // Atraso inicial para estabilização
    stdio_init_all();
    printf("Initializing...");

    // Inicializar LED
    initLeds();

    // Inicializar I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    // Inicializar MPU6050
    initMPU6050();
    printf("Inicializando MPU6050...\n");

    sleep_ms(1000);

    while (true)
    {
        // Ler sensores
        updateAccelerometerData(&ax, &ay, &az);
        calculateInclinationAngles(&roll, &pitch);

        printf("Roll (X): %.2f° | Pitch (Y): %.2f° \n", roll, pitch);
        setLedBrightness(LED_RED_PIN, 127 + (int)(roll * 127 / 90));    // Ajusta brilho do LED vermelho baseado no roll
        setLedBrightness(LED_GREEN_PIN, 127 + (int)(pitch * 127 / 90)); // Ajusta brilho do LED verde baseado no pitch

        sleep_ms(64); // Atualiza a cada 0.5 segundo
    }
}