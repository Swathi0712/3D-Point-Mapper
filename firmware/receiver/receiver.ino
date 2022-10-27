/**
 * 3D Point Mapper - Receiver
 * 
 * ESP8266-based receiver that collects 3D coordinates from the transmitter
 * via NRF24L01 radio module and serves them through a web interface.
 * 
 * Author: [Your Name]
 * Date: May 2025
 * License: MIT
 */

 #include <Arduino.h>
 #include <ESP8266WiFi.h>
 #include <ESPAsyncTCP.h>
 #include <ESPAsyncWebServer.h>
 #include <FS.h>
 #include <SPI.h>
 #include <nRF24L01.h>
 #include <RF24.h>
 #include <ArduinoJson.h>
 #include <WiFiManager.h>
 
 #include "config.h"
 #include "radio.h"
 #include "webserver.h"
 
 // Object instances
 RF24 radio(CE_PIN, CSN_PIN);
 AsyncWebServer server(80);
 DataPackage data;
 
 // Global variables
 String dataString = "0 0 0";  // Formatted data string for web client
 bool radioConnected = false;  // Radio connection status
 bool receivingData = false;   // Data reception status
 unsigned long packetCount = 0; // Count of received packets
 unsigned long errorCount = 0;  // Count of errors
 
 // Timer variables
 unsigned long lastReceiveTime = 0;
 unsigned long lastStatsTime = 0;
 unsigned long startTime = 0;
 
 void setup() {
   // Initialize serial communication
   Serial.begin(115200);
   Serial.println(F("\n\n3D Point Mapper - Receiver"));
   
   // Initialize file system
   if (!SPIFFS.begin()) {
     Serial.println(F("Error mounting SPIFFS"));
     return;
   }
   
   // Initialize WiFi using WiFiManager
   WiFiManager wifiManager;
   wifiManager.autoConnect("3DPointMapper");
   
   Serial.println("Connected to WiFi");
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());
   
   // Initialize NRF24L01 radio
   if (!initRadio()) {
     Serial.println(F("Radio initialization failed"));
   } else {
     Serial.println(F("Radio initialized successfully"));
   }
   
   // Setup web server routes
   setupWebServer();
   
   // Start web server
   server.begin();
   Serial.println(F("Web server started"));
   
   // Record start time
   startTime = millis();
 }
 
 void loop() {
   // Check for incoming radio data
   checkRadioData();
   
   // Check connection status
   checkConnectionStatus();
   
   // Update statistics periodically
   if (millis() - lastStatsTime > STATS_INTERVAL) {
     printStatistics();
     lastStatsTime = millis();
   }
 }
 
 // Check for incoming radio data
 void checkRadioData() {
   if (radio.available()) {
     radioConnected = true;
     
     // Read the data packet
     radio.read(&data, sizeof(DataPackage));
     
     // Throttle updates to avoid flooding the web client
     if (millis() - lastReceiveTime > DATA_UPDATE_INTERVAL) {
       lastReceiveTime = millis();
       receivingData = true;
       packetCount++;
       
       // Format data for web client
       dataString = String(data.x) + " " + String(data.y) + " " + String(data.z);
       
       // Print to serial for debugging
       Serial.println(dataString);
     }
   }
 }
 
 // Check connection status and reset if needed
 void checkConnectionStatus() {
   // Check if we haven't received data for a while
   if (receivingData && millis() - lastReceiveTime > CONNECTION_TIMEOUT) {
     receivingData = false;
     Serial.println("Warning: No data received recently");
     errorCount++;
     
     // Try to restart radio
     radio.stopListening();
     delay(10);
     radio.startListening();
   }
 }
 
 // Print statistics to serial monitor
 void printStatistics() {
   unsigned long uptime = (millis() - startTime) / 1000; // Uptime in seconds
   
   Serial.println("--------- STATUS ---------");
   Serial.printf("Uptime: %lu seconds\n", uptime);
   Serial.printf("Radio connected: %s\n", radioConnected ? "Yes" : "No");
   Serial.printf("Receiving data: %s\n", receivingData ? "Yes" : "No");
   Serial.printf("Packets received: %lu\n", packetCount);
   Serial.printf("Errors: %lu\n", errorCount);
   Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
   Serial.println("-------------------------");
 }