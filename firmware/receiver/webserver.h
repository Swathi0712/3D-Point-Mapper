// webserver.h
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>

// Web server
extern AsyncWebServer server;

// Setup web server routes
void setupWebServer() {
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html");
  });
  
  // Route for data endpoint
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    extern String dataString;
    request->send_P(200, "text/plain", dataString.c_str());
  });
  
  // Route for status endpoint
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    extern bool radioConnected, receivingData;
    extern unsigned long packetCount, errorCount;
    
    // Create JSON response
    DynamicJsonDocument doc(200);
    doc["connected"] = radioConnected;
    doc["receiving"] = receivingData;
    doc["packets"] = packetCount;
    doc["errors"] = errorCount;
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });
  
  // Route for static files
  server.serveStatic("/", SPIFFS, "/");
}

#endif // WEBSERVER_H