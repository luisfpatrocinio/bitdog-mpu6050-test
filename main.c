#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <math.h>
#include <stdio.h>

// Patrolibs
#include "led.h"

// Project Libs
#include "gyro.h"
#include "wifi_udp.h"

// Definições de GPIOs e endereço do MPU6050
#define I2C_PORT i2c1
#define MPU6050_ADDR 0x68
#define SDA_PIN 2
#define SCL_PIN 3

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
        // setLedBrightness(LED_RED_PIN, 127 + (int)(roll * 127 / 90));    // Ajusta brilho do LED vermelho baseado no roll
        // setLedBrightness(LED_GREEN_PIN, 127 + (int)(pitch * 127 / 90)); // Ajusta brilho do LED verde baseado no pitch

        // Printar numeros inteiros de acordo com os valores de roll e pitch, simulando um dado:
        int roll_int = (int)(roll / 90 * 6);   // Mapeia roll para 0-5
        int pitch_int = (int)(pitch / 90 * 6); // Mapeia pitch para 0-5
        printf("Dado Roll: %d | Dado Pitch: %d\n", roll_int, pitch_int);

        setAllLedsBrightness(0); // Desliga todos os LEDs

        if (roll_int == -5)
        {
            setLedBrightness(LED_RED_PIN, 255);
        }

        if (roll_int == 5)
        {
            setLedBrightness(LED_RED_PIN, 255);
        }

        if (pitch_int == -5)
        {
            setLedBrightness(LED_GREEN_PIN, 255);
        }

        if (pitch_int == 5)
        {
            setLedBrightness(LED_GREEN_PIN, 255);
        }

        if (pitch_int == 0 && roll_int == 0)
        {
            setLedBrightness(LED_BLUE_PIN, 255); // Liga o LED azul se ambos forem zero
        }

        if (pitch_int == -11 || pitch_int == 11 || roll_int == -11 || roll_int == 11)
        {
            setLedBrightness(LED_BLUE_PIN, 255);
            setLedBrightness(LED_RED_PIN, 255);
            setLedBrightness(LED_GREEN_PIN, 255);
        }

        sleep_ms(200);
    }
}