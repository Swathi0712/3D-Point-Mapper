// config.h
#ifndef CONFIG_H
#define CONFIG_H

// Pin definitions
#define TRIGGER_PIN 4     // Ultrasonic trigger pin
#define ECHO_PIN 5        // Ultrasonic echo pin
#define CE_PIN 7          // NRF24L01 CE pin
#define CSN_PIN 8         // NRF24L01 CSN pin

// Display settings
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_ADDRESS 0x3C // I2C address for OLED

// Timing constants
#define DISPLAY_UPDATE_INTERVAL 100  // Display update interval (ms)
#define TRANSMISSION_INTERVAL 50     // Data transmission interval (ms)
#define ULTRASONIC_SAMPLING_RATE 20  // Ultrasonic reading rate (Hz)
#define ULTRASONIC_TIMEOUT 30000     // Timeout for ultrasonic readings (µs)

// Filtering constants
#define DISTANCE_SMOOTHING_FACTOR 0.3  // Low-pass filter factor for distance

// Radio settings
extern const byte addresses[][6];

#endif // CONFIG_H