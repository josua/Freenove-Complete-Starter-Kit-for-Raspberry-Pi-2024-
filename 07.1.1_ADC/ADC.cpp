/*
Filename    : ADC.cpp
Description : Use ADC module to read the voltage value of potentiometer.
Author      : Philippe Jos
Modified    : 06/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/07.1.1_ADC
*/
#include <stdio.h>                                          // Include standard I/O library
#include <wiringPi.h>                                       // Include WiringPi library for GPIO control
#include <ADCDevice.hpp>                                    // Include ADCDevice header

ADCDevice *adc;                                             // Define an ADC Device class object

int setup() {
    adc = new ADCDevice();                                  // Initialize ADCDevice object

    if (adc->detectI2C(0x48)) {                             // Detect the pcf8591
        delete adc;                                         // Free previously pointed memory
        adc = new PCF8591();                                // If detected, create an instance of PCF8591
    } else if (adc->detectI2C(0x4b)) {                      // Detect the ads7830
        delete adc;                                         // Free previously pointed memory
        adc = new ADS7830();                                // If detected, create an instance of ADS7830
    } else {                                                // No correct I2C address found
        printf("No correct I2C address found,\n"
               "Please use command 'i2cdetect -y 1' to check the I2C address!\n"
               "Program Exit.\n");                          // Print error message
        return -1;                                          // Return -1 to indicate error
    }
    return 0;                                               // Return 0 to indicate success
}

void loop() {
    int adcValue = adc->analogRead(0);                      // Read analog value of A0 pin
    float voltage = (float)adcValue / 255.0 * 3.3;          // Calculate voltage
    printf("ADC value: %d \tVoltage: %.2fV\n", adcValue, voltage); // Print ADC value and voltage
    delay(100);                                             // Delay 100 milliseconds
}

int main(void) {
    if (setup() == -1) return -1;                           // Call setup function, exit if it returns -1
    while (1) {                                             // Infinite loop
        loop();                                             // Call loop function
    }
    return 0;                                               // Return 0 (never reached)
}

