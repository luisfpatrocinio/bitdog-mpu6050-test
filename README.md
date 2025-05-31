# bitdog-mpu6050-test

A simple project to test and visualize MPU6050 sensor readings using a Raspberry Pi Pico. The project reads roll and pitch angles from the MPU6050 accelerometer and uses them to control LED brightness in real-time.

## Features

- Real-time reading of MPU6050 accelerometer data
- Calculation of roll and pitch angles
- Visual feedback using RGB LEDs
- High update rate (15.6Hz) for smooth readings

## Hardware Requirements

- Raspberry Pi Pico
- MPU6050 sensor
- RGB LEDs
- I2C connections (SDA: GPIO2, SCL: GPIO3)

## Pin Configuration

- MPU6050:
  - SDA: GPIO2
  - SCL: GPIO3
- LEDs:
  - Red LED: Shows roll angle
  - Green LED: Shows pitch angle

## Usage

1. Connect the hardware according to the pin configuration
2. Flash the code to your Raspberry Pi Pico
3. The program will start reading sensor data and updating LED brightness based on the inclination angles

## 📄 License

This project is licensed under the MIT License.  
Feel free to use it in your projects -- just make sure to credit **Luis Felipe Patrocinio** ([GitHub](https://github.com/luisfpatrocinio)).  
See the [LICENSE](./LICENSE) file for full details.