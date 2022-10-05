// sensors.h
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <MPU6050_light.h>
#include "config.h"

// Global variables
extern MPU6050 mpu;
extern float distance;
extern bool sensorError;

// Initialize MPU6050
bool initMPU() {
  Wire.begin();
  byte status = mpu.begin();
  if (status != 0) {
    Serial.println(F("MPU6050 initialization failed"));
    return false;
  }
  
  // Run MPU6050 calibration
  mpu.calcGyroOffsets();
  return true;
}

// Read distance from ultrasonic sensor using non-blocking approach
void readUltrasonicSensor() {
  static enum {IDLE, TRIGGERED, WAITING_FOR_ECHO} state = IDLE;
  static unsigned long triggerTime;
  static unsigned long lastReadingTime = 0;
  
  // State machine for non-blocking ultrasonic reading
  switch (state) {
    case IDLE:
      // Check if it's time for a new reading
      if (millis() - lastReadingTime >= (1000 / ULTRASONIC_SAMPLING_RATE)) {
        // Send trigger pulse
        digitalWrite(TRIGGER_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIGGER_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIGGER_PIN, LOW);
        
        triggerTime = micros();
        state = TRIGGERED;
      }
      break;
      
    case TRIGGERED:
      // Wait for echo pin to go HIGH or timeout
      if (digitalRead(ECHO_PIN) == HIGH) {
        triggerTime = micros();
        state = WAITING_FOR_ECHO;
      } else if (micros() - triggerTime > 5000) { // 5ms timeout
        // Timeout waiting for echo to start
        state = IDLE;
      }
      break;
      
    case WAITING_FOR_ECHO:
      // Wait for echo pin to go LOW or timeout
      if (digitalRead(ECHO_PIN) == LOW) {
        // Calculate distance
        long duration = micros() - triggerTime;
        float newDistance = (duration / 2.0) / 29.1; // Convert to cm
        
        // Apply simple filtering for noise reduction
        if (newDistance >= 2 && newDistance <= 400) {
          // Simple low-pass filter (smoothing)
          distance = (1.0 - DISTANCE_SMOOTHING_FACTOR) * distance + 
                      DISTANCE_SMOOTHING_FACTOR * newDistance;
        }
        
        lastReadingTime = millis();
        state = IDLE;
      } else if (micros() - triggerTime > ULTRASONIC_TIMEOUT) {
        // Timeout waiting for echo to end
        state = IDLE;
      }
      break;
  }
}

// Recalibrate the MPU6050 gyroscope
void calibrateMPU() {
  Serial.println(F("Recalibrating MPU6050..."));
  Serial.println(F("Keep device still!"));
  
  // Perform calibration
  mpu.calcGyroOffsets();
  
  Serial.println(F("Calibration done!"));
}

#endif // SENSORS_H