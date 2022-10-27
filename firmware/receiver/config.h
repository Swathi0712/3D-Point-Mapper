// config.h
#ifndef CONFIG_H
#define CONFIG_H

// Pin definitions
#define CE_PIN 4   // D2 on NodeMCU
#define CSN_PIN 5  // D1 on NodeMCU

// Timing constants
#define CONNECTION_TIMEOUT 5000  // 5 seconds timeout
#define STATS_INTERVAL 5000      // Statistics update interval (ms)
#define DATA_UPDATE_INTERVAL 10  // Minimum interval between data updates (ms)

// Data constants
#define DATA_HISTORY_SIZE 100  // Number of data points to keep in history

// Radio settings
extern const byte addresses[][6];

#endif // CONFIG_H