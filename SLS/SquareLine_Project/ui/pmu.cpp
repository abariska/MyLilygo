// pmu.cpp
#include "pmu.h"
#include <Wire.h>

// Power Management Unit instance
static PowersSY6970 PMU;
static bool hasPMU = false;
static float lastPercentage = -1; // To store the last battery percentage

// Global variables
float batteryVoltage = 0.0;   // Battery voltage in volts
int batteryPercentage = 0;    // Battery percentage
bool isCharging = false;      // Charging status

// Function to calculate battery percentage based on voltage
int calculateBatteryPercentage(float voltage) {
    const float minVoltage = 3.0;   // Battery empty
    const float maxVoltage = 4.2;   // Battery fully charged

    if (voltage < minVoltage) voltage = minVoltage;
    if (voltage > maxVoltage) voltage = maxVoltage;

    // Linear interpolation
    int percentage = (int)((voltage - minVoltage) / (maxVoltage - minVoltage) * 100);

    // Hysteresis: Only update the percentage if it changes significantly (e.g., by 2%)
    if (lastPercentage == -1 || abs(percentage - lastPercentage) > 2) {
        lastPercentage = percentage;
    }

    return lastPercentage;
}

// Initialize PMU for battery management
bool initPMU() {
    hasPMU = PMU.init(Wire, BOARD_I2C_SDA, BOARD_I2C_SCL, SY6970_SLAVE_ADDRESS);
    if (!hasPMU) {
        Serial.println("PMU is not online...");
        return false;
    } else {
        PMU.disableOTG();          // Disable OTG (USB Power Out)
        PMU.enableADCMeasure();     // Enable ADC for measurement
        PMU.enableCharge();         // Enable charging
        return true;
    }
}

// Function to update PMU status and store values in global variables
void updatePMU() {
    if (!hasPMU) return;

    // Get battery voltage and charging status
    isCharging = PMU.isVbusIn();              // Check if USB is connected (charging)
    float battVoltage = PMU.getBattVoltage(); // Get battery voltage in millivolts
    batteryVoltage = battVoltage / 1000.0;    // Convert to volts

    // Calculate battery percentage
    batteryPercentage = calculateBatteryPercentage(batteryVoltage);

    // Print battery voltage, percentage, and charging status to Serial
    Serial.print("Battery Voltage: ");
    Serial.print(batteryVoltage);  // Already in volts for logging
    Serial.print("V, Percentage: ");
    Serial.print(batteryPercentage);
    Serial.print("%, Charging: ");
    Serial.println(isCharging ? "Yes" : "No");
}
