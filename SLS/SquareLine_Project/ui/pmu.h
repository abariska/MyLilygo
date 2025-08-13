// pmu.h
#ifndef PMU_H
#define PMU_H

#include "utilities.h"
#include <XPowersLib.h>

// Global variables to store battery data
extern float batteryVoltage;  // Battery voltage in volts
extern int batteryPercentage; // Battery percentage
extern bool isCharging;       // Charging status

// Initialize PMU for battery management
bool initPMU();

// Function to update PMU status and store values in global variables
void updatePMU();

#endif
