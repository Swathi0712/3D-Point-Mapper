// coordinates.h
#ifndef COORDINATES_H
#define COORDINATES_H

#include <Arduino.h>
#include "config.h"

// Data structure for radio transmission (max 32 bytes)
struct DataPackage {
  short int x = 0;  // X coordinate
  short int y = 0;  // Y coordinate
  short int z = 0;  // Z coordinate
};

// Convert spherical coordinates to cartesian
void calculateCoordinates(DataPackage& data, float distance, float angleY, float angleZ) {
  // Convert degrees to radians
  float pitch = angleY * PI / 180.0;
  float yaw = angleZ * PI / 180.0;
  
  // Calculate 3D coordinates from spherical coordinates
  // X = distance * cos(pitch) * cos(yaw)
  // Y = distance * cos(pitch) * sin(yaw)
  // Z = distance * sin(pitch)
  data.x = (short int)(distance * cos(pitch) * cos(yaw));
  data.y = (short int)(distance * cos(pitch) * sin(yaw));
  data.z = (short int)(distance * sin(pitch));
}

#endif // COORDINATES_Hc