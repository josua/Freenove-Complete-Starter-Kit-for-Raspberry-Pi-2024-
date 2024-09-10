/*
Filename    : Thermometer.cpp
Description : Thermometer
Author      : Phlippe Jos
Modified    : 09/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/11.1.1_Thermometer
*/
#include <stdio.h>                                                        // Include standard I/O library
#include <memory>                                                         // Include memory management for smart pointers
#include <cmath>                                                          // Include cmath for C++ compatibility
#include <wiringPi.h>                                                     // Include WiringPi library
#include <ADCDevice.hpp>                                                  // Include ADC device class

// Pre-computed ratio
const float VOLTAGE_RATIO = 3.3 / 255.0;                                  // Define ratio to map ADC value to voltage

// Sensor I2C addresses
const int PCF8591_ADDRESS = 0x48;                                         // I2C address for PCF8591
const int ADS7830_ADDRESS = 0x4b;                                         // I2C address for ADS7830

std::unique_ptr<ADCDevice> adc;                                           // Use smart pointer for automatic memory management

// Setup function for initializing ADC device
int setup() {
    adc = std::make_unique<ADCDevice>();                                  // Initialize ADC device

    if (adc->detectI2C(PCF8591_ADDRESS)) {                                // Detect the PCF8591 Module
        adc = std::make_unique<PCF8591>();                                // Create an instance of PCF8591 if detected
    } else if (adc->detectI2C(ADS7830_ADDRESS)) {                         // Detect the ADS7830 Module
        adc = std::make_unique<ADS7830>();                                // Create an instance of ADS7830 if detected
    } else {
        fprintf(stderr, "No correct I2C address found.\n"
                        "Please use command 'i2cdetect -y 1' to check the I2C address.\n"
                        "Program Exit.\n");
        return -1;                                                        // Exit setup if no valid I2C address is found
    }

    return 0;                                                             // Return 0 if setup is successful
}

// Loop function to read analog value and compute temperature
void loop() {
    int adcValue = adc->analogRead(0);                                     // Read analog value from A0 pin    
    if (adcValue == -1) {                                                  // Check for read errors
        fprintf(stderr, "Error reading from ADC.\n");
        return;                                                            // Exit loop on read error
    }
    
    float voltage = static_cast<float>(adcValue) * VOLTAGE_RATIO;          // Calculate voltage
    float Rt = 10 * voltage / (3.3 - voltage);                             // Calculate resistance value of thermistor
    float tempK = 1 / (1 / (273.15 + 25) + log(Rt / 10) / 3950.0);         // Compute temperature in Kelvin
    float tempC = tempK - 273.15;                                          // Convert temperature to Celsius
    
    printf("ADC: %d \tVoltage: %.2fV  \tTemperature: %.2f°C\n", 
           adcValue, voltage, tempC);                                      // Print ADC, voltage, and temperature values
    delay(1000);                                                           // Delay for 1 second
}

int main(void) {
    if (setup() == -1) return -1;                                          // Check if setup was successful
    while (true) loop();                                                   // Loop indefinitely
    return 0;                                                              // Return 0 (never reached)
}



