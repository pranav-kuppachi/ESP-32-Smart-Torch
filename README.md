# ESP32 Smart Torch

A small ESP32-based smart lighting system that changes LED output based on ambient light, with a manual brightness control mode.

The project was built and tested as a simulated embedded system using [Wokwi](https://wokwi.com/).

## Features

### Automatic Mode

An LDR is used to measure the surrounding light level. The ESP32 processes the sensor reading and maps it to a brightness range.

The system then selects the appropriate LED based on the calculated level:

| Light Level | LED   |
| ----------- | ----- |
| 0–33%       | White |
| 34–66%      | Cyan  |
| 67–100%     | Blue  |

### Manual Mode

A potentiometer provides manual control over the output level.

The MODE button switches between automatic and manual operation.

## Hardware

The simulated circuit uses:

* ESP32 DevKit
* LDR / photoresistor
* Potentiometer
* Push button
* Blue LED
* Cyan LED
* White LED
* 220Ω resistors

## Pin Configuration

| Component     | ESP32 GPIO |
| ------------- | ---------: |
| LDR           |    GPIO 35 |
| Potentiometer |    GPIO 34 |
| MODE button   |    GPIO 14 |
| Blue LED      |    GPIO 25 |
| White LED     |    GPIO 26 |
| Cyan LED      |    GPIO 27 |

## How It Works

The ESP32 continuously reads either the LDR or potentiometer depending on the current mode.

The analog input is converted into a percentage from 0–100%.

That value is then mapped to an LED output using ESP32 PWM.

The project uses the ESP32 LEDC peripheral to control LED output.

## Wokwi Simulation

The complete circuit can be simulated without physical hardware.

**[Run the project on Wokwi](https://wokwi.com/projects/476421595544480769)**

## License

This project is for educational and portfolio purposes.
