// radio.h
#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "config.h"

// Radio module
extern RF24 radio;

// Radio addresses
const byte addresses[][6] = {"00001", "00002"};

// Data structure for radio reception (max 32 bytes)
struct DataPackage {
  short int x = 0;
  short int y = 0;
  short int z = 0;
};

// Initialize radio
bool initRadio() {
  if (!radio.begin()) {
    return false;
  }
  
  radio.openReadingPipe(1, addresses[1]);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
  return true;
}

#endif // RADIO_H