#include "hardware/i2c.h"
#include <math.h>
#include <pico/time.h>
#include <stdio.h>

// Function prototypes
int printf(const char *format, ...);

// Patrolibs
#include "led.h"

// Project Libs
#include "gyro.h"
#include "patroGyroTest.h"
#include "wifi_udp.h"

// Definições de GPIOs e endereço do MPU6050
#define I2C_PORT i2c1
#define MPU6050_ADDR 0x68
#define SDA_PIN 2
#define SCL_PIN 3

// Global Variables
int connectedToGame = 0; // Flag to indicate if connected to the game

// Callback UDP
void udpReceiveCallback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
  // printf("UDP Received\n");
  char *data = (char *)p->payload;
  data[p->len] = '\0'; // Null-terminate the string

  printf("Received UDP packet from %s:%d: %s\n", ipaddr_ntoa(addr), port, data);

  gTargetIP =
      *addr; // Update the global target IP address with the sender's address

  // Check if the received data is a handshake message
  if (strcmp(data, "udp_handshake") == 0) {
    printf("UDP Handshake received, sending ack...\n");
    // Send an acknowledgment back to the sender
    sendUDP("udp_handshake_ack");
    connectedToGame = 1;
  }

  // Free the received pbuf
  pbuf_free(p);
}

int main() {
  // Inicialização do Programa
  sleep_ms(690);
  printf("Initializing...");
  stdio_init_all();

  // Inicializar LED
  printf("Initializing LEDS...\n");
  initLeds();

  // Inicializar I2C
  printf("Initializing I2C...\n");
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

  // Inicializar MPU6050
  printf("Initializing MPU6050...\n");
  initMPU6050();

  // Inicializar WiFi
  printf("Initializing WiFi...\n");
  wifiSetup();
  wifiConnectAsync(WIFI_SSID, WIFI_PASSWORD);

  printf("Waiting for WiFi connection...\n");
  while (!wifiIsConnected()) {
    static int timeout = 0;
    pulseLed(LED_RED_PIN, 0.10); 

    timeout += 1;
    if (timeout > 500) { // Timeout after 10 seconds
      printf("WiFi connection timed out.\n");
      setLedBrightness(LED_RED_PIN, 255);
      setLedBrightness(LED_GREEN_PIN, 0);
      sleep_ms(1000);
      printf("Retrying...\n");
      timeout = 0;
      wifiConnectAsync(WIFI_SSID, WIFI_PASSWORD);
    }
  }

  printf("WiFi network connection established.\n");
  setLedBrightness(LED_RED_PIN, 0);
  setLedBrightness(LED_GREEN_PIN, 255);

  sleep_ms(1000);
  setLedBrightness(LED_GREEN_PIN, 0);

  // Create a UDP PCB (Protocol Control Block)
  gPCB = udp_new();
  if (!gPCB) {
    printf("Failed to create UDP PCB\n");
    return 1;
  }

  // Set up the UDP receive callback to handle incoming packets
  openUDPBind();
  udp_recv(gPCB, udpReceiveCallback, NULL);

  // Wait for UDP handshake
  printf("Waiting for UDP handshake...\n");
  while (!connectedToGame) {
    pulseLed(LED_GREEN_PIN, 0.20);
  }

  // Indicate successful connection to the game
  if (connectedToGame) {
    printf("Connected to the game via UDP!\n");
    setLedBrightness(LED_GREEN_PIN, 255);
    setLedBrightness(LED_RED_PIN, 0);
    sleep_ms(269);
  }
  turnOffLeds();
  sleep_ms(269);

  // Cube Initialization
  MPU6050_data_t sensor_data;    // Declare a struct to hold sensor data
  initOrientation(&sensor_data); // Initialize the sensor data structure

  while (true) {
    // Ler sensores
    updateOrientation(&sensor_data);

    // Calcular ângulos de inclinação
    calculateInclinationAngles(&sensor_data);

    // printf("Roll (X): %.2f° | Pitch (Y): %.2f° \n", sensor_data.roll,
    // sensor_data.pitch);

    // Printar numeros inteiros de acordo com os valores de roll e pitch,
    // simulando um dado:
    int roll_int = (int)(sensor_data.roll / 90 * MAX_ROLL);   // Mapeia roll para 0-5
    int pitch_int = (int)(sensor_data.pitch / 90 * MAX_ROLL); // Mapeia pitch para 0-5
    int yaw_int = (int)(sensor_data.yaw / 90 * MAX_ROLL);     // Mapeia yaw para 0-5
    // printf("Roll: %d | Pitch: %d | Yaw: %d\n", roll_int, pitch_int,
    // yaw_int);

    // Get and send Cube Face
    current_face = getCubeFace(sensor_data.roll, sensor_data.pitch);
    // printf("Current Cube Face: %d\n", current_face);
    char face_str[32];
    snprintf(face_str, sizeof(face_str), "C|%d", (int)current_face);
    sendUDP(face_str);

    // Get and send Roll and Pitch
    char roll_pitch_str[32];
    snprintf(roll_pitch_str, sizeof(roll_pitch_str), "R|%d|%d", roll_int, pitch_int);
    sendUDP(roll_pitch_str);

    updateLedsByRollAndPitch(roll_int, pitch_int);

    sleep_ms(169);
  }
}

// Unused
// setLedBrightness(LED_RED_PIN, 127 + (int)(roll * 127 / 90));    // Ajusta
// brilho do LED vermelho baseado no roll setLedBrightness(LED_GREEN_PIN, 127
// + (int)(pitch * 127 / 90)); // Ajusta brilho do LED verde baseado no pitch