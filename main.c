#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <math.h>
#include <stdio.h>

// Patrolibs
#include "led.h"

// Project Libs
#include "gyro.h"
#include "wifi_udp.h"
#include "patroGyroTest.h"

// Definições de GPIOs e endereço do MPU6050
#define I2C_PORT i2c1
#define MPU6050_ADDR 0x68
#define SDA_PIN 2
#define SCL_PIN 3

// Callback UDP
void udpReceiveCallback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    printf("UDP Received\n");
    char *data = (char *)p->payload;
    data[p->len] = '\0'; // Null-terminate the string

    printf("Received UDP packet from %s:%d: %s\n", ipaddr_ntoa(addr), port, data);

    gTargetIP = *addr; // Update the global target IP address with the sender's address

    // Process the received data here if needed
    if (strcmp(data, "searching") == 0)
    {
        printf("Received 'searching' message. Sending 'connected' back.\n");
        sendUDP("connected");
    }

    // Free the received pbuf
    pbuf_free(p);
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

    // Inicializar WiFi
    wifiSetup();
    wifiConnectAsync(WIFI_SSID, WIFI_PASSWORD);

    printf("Aguardando conexão com a rede WiFi...\n");
    while (!wifiIsConnected())
    {
        static float time = 0.0f;
        time += 0.1f;                                         // Incremento suave para a animação
        setLedBrightness(LED_RED_PIN, 127 + sin(time) * 127); // LED vermelho piscando entre 0 e 255
        sleep_ms(20);
    }

    printf("Conexão com a rede WiFi estabelecida.\n");
    setLedBrightness(LED_RED_PIN, 0);
    setLedBrightness(LED_GREEN_PIN, 255);
    sleep_ms(1000);
    setLedBrightness(LED_GREEN_PIN, 0);

    // Create a UDP PCB (Protocol Control Block)
    gPCB = udp_new();
    if (!gPCB)
    {
        printf("Failed to create UDP PCB\n");
        printf("PCB fail\n");
        // drawError("PCB fail");
        return 1;
    }

    // Set up the UDP receive callback to handle incoming packets
    openUDPBind();
    udp_recv(gPCB, udpReceiveCallback, NULL);

    sleep_ms(1000);

    MPU6050_data_t sensor_data; // Declare a struct to hold sensor data

    while (true)
    {
        // Ler sensores
        updateAccelerometerData(&sensor_data);
        calculateInclinationAngles(&sensor_data);

        printf("Roll (X): %.2f° | Pitch (Y): %.2f° \n", sensor_data.roll, sensor_data.pitch);

        // Printar numeros inteiros de acordo com os valores de roll e pitch, simulando um dado:
        int roll_int = (int)(sensor_data.roll / 90 * 6);   // Mapeia roll para 0-5
        int pitch_int = (int)(sensor_data.pitch / 90 * 6); // Mapeia pitch para 0-5
        printf("Dado Roll: %d | Dado Pitch: %d\n", roll_int, pitch_int);

        updateLedsByRollAndPitch(roll_int, pitch_int);

        // Send Roll and Pitchdata via udp
        char udpMsg[32];
        snprintf(udpMsg, sizeof(udpMsg), "R|%d|%d", roll_int, pitch_int);
        sendUDP(udpMsg);

        sleep_ms(169);
    }
}

// Unused
// setLedBrightness(LED_RED_PIN, 127 + (int)(roll * 127 / 90));    // Ajusta brilho do LED vermelho baseado no roll
// setLedBrightness(LED_GREEN_PIN, 127 + (int)(pitch * 127 / 90)); // Ajusta brilho do LED verde baseado no pitch