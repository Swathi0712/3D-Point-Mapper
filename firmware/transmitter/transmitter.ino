/**
 * 3D Point Mapper - Transmitter
 * 
 * Main program file for the transmitter unit.
 * Combines ultrasonic sensor with MPU6050 gyroscope to create a 3D mapping system.
 * 
 * Author: [Your Name]
 * Date: May 2025
 * License: MIT
 */

 #include <Wire.h>
 #include <Adafruit_GFX.h>
 #include <Adafruit_SSD1306.h>
 #include <MPU6050_light.h>
 #include <SPI.h>
 #include <nRF24L01.h>
 #include <RF24.h>
 
 #include "config.h"
 #include "coordinates.h"
 #include "sensors.h"
 #include "radio.h"
 
 // Object instances
 Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
 MPU6050 mpu(Wire);
 RF24 radio(CE_PIN, CSN_PIN);
 DataPackage data;
 
 // Global variables
 float distance = 0;    // Current distance reading in cm
 bool sensorError = false; // Flag for sensor errors
 
 // Timing variables
 unsigned long lastDisplayUpdate = 0;
 unsigned long lastTransmission = 0;
 
 void setup() {
   // Initialize serial communication
   Serial.begin(115200);
   Serial.println(F("3D Point Mapper - Transmitter"));
   
   // Setup ultrasonic pins
   pinMode(TRIGGER_PIN, OUTPUT);
   pinMode(ECHO_PIN, INPUT);
   digitalWrite(TRIGGER_PIN, LOW);
   
   // Initialize display
   if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
     Serial.println(F("SSD1306 display initialization failed"));
     sensorError = true;
   } else {
     display.clearDisplay();
     display.setTextSize(1);
     display.setTextColor(SSD1306_WHITE);
     display.setCursor(0, 0);
     display.println(F("3D Point Mapper"));
     display.println(F("Initializing..."));
     display.display();
   }
   
   // Initialize MPU6050
   if (!initMPU()) {
     sensorError = true;
   }
   
   // Initialize radio
   if (!initRadio()) {
     sensorError = true;
   }
   
   // Final setup message
   display.clearDisplay();
   display.setCursor(0, 0);
   display.setTextSize(2);
   
   if (sensorError) {
     display.println(F("ERROR!"));
     display.setTextSize(1);
     display.println(F("Check connections"));
   } else {
     display.println(F("Ready!"));
     display.setTextSize(1);
     display.println(F("Mapping started..."));
   }
   display.display();
   delay(1000);
 }
 
 void loop() {
   // Update MPU data
   mpu.update();
   
   // Read ultrasonic sensor (non-blocking)
   readUltrasonicSensor();
   
   // Calculate 3D coordinates
   calculateCoordinates(data, distance, mpu.getAngleY(), mpu.getAngleZ());
   
   // Update display periodically
   if (millis() - lastDisplayUpdate > DISPLAY_UPDATE_INTERVAL) {
     updateDisplay();
     lastDisplayUpdate = millis();
   }
   
   // Transmit data periodically
   if (millis() - lastTransmission > TRANSMISSION_INTERVAL) {
     if (transmitData(data)) {
       Serial.print(data.x);
       Serial.print(" ");
       Serial.print(data.y);
       Serial.print(" ");
       Serial.println(data.z);
     }
     lastTransmission = millis();
   }
 }
 
 // Update OLED display with current readings
 void updateDisplay() {
   display.clearDisplay();
   display.setCursor(0, 0);
   display.setTextSize(1);
   
   // Show distance reading
   display.print(F("Dist: "));
   display.print(distance, 1);
   display.println(F(" cm"));
   
   // Show angles
   display.print(F("Pitch: "));
   display.print(mpu.getAngleY(), 1);
   display.println(F(" deg"));
   
   display.print(F("Yaw: "));
   display.print(mpu.getAngleZ(), 1);
   display.println(F(" deg"));
   
   // Show coordinates
   display.print(F("X:"));
   display.print(data.x);
   display.print(F(" Y:"));
   display.print(data.y);
   display.print(F(" Z:"));
   display.println(data.z);
   
   // Show status
   display.print(F("Status: "));
   if (sensorError) {
     display.println(F("ERROR"));
   } else {
     display.println(F("OK"));
   }
   
   display.display();
 }