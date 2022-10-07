// radio.h
#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "config.h"
#include "coordinates.h"

// Radio module
extern RF24 radio;

// Radio addresses
const byte addresses[][6] = {"00001", "00002"};

// Initialize radio
bool initRadio() {
  if (!radio.begin()) {
    Serial.println(F("Radio initialization failed"));
    return false;
  }
  
  radio.openWritingPipe(addresses[1]);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  return true;
}

// Transmit data package
bool transmitData(DataPackage& data) {
  return radio.write(&data, sizeof(DataPackage));
}

#endif // RADIO_H