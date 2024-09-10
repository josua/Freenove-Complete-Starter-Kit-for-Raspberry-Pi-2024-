/*
Filename    : Softlight.cpp
Description : Use potentiometer to control LED
Author      : Philippe Jos
Modified    : 06/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/08.1.1_Softlight
*/
#include <stdio.h>                                           // Include standard I/O library
#include <wiringPi.h>                                        // Include WiringPi library for GPIO control
#include <softPwm.h>                                         // Include softPwm library for PWM control
#include <ADCDevice.hpp>                                     // Include ADCDevice header

#define LED_PIN 0                                            // Define the LED pin

ADCDevice *adc;                                              // Define an ADC Device class object

// Function to set up the ADC device and initialize the LED pin
int setup() {
    adc = new ADCDevice();                                   // Initialize ADCDevice object
    
    if (adc->detectI2C(0x48)) {                              // Detect the pcf8591
        delete adc;                                          // Free previously pointed memory
        adc = new PCF8591();                                 // If detected, create an instance of PCF8591
    } else if (adc->detectI2C(0x4b)) {                       // Detect the ads7830
        delete adc;                                          // Free previously pointed memory
        adc = new ADS7830();                                 // If detected, create an instance of ADS7830
    } else {                                                 // No correct I2C address found
        printf("No correct I2C address found,\n"
               "Please use command 'i2cdetect -y 1' to check the I2C address!\n"
               "Program Exit.\n");                           // Print error message
        return -1;                                           // Return -1 to indicate error
    }
    wiringPiSetup();                                         // Initialize wiringPi
    softPwmCreate(LED_PIN, 0, 100);                          // Create SoftPWM pin for LED_PIN
    return 0;                                                // Return 0 to indicate success
}

// Function to read the ADC value, update the PWM duty cycle, and print the voltage
void loop() {
    int adcValue = adc->analogRead(0);                       // Read analog value of A0 pin
    softPwmWrite(LED_PIN, adcValue * 100 / 255);             // Mapping to PWM duty cycle
    float voltage = (float)adcValue / 255.0 * 3.3;           // Calculate voltage
    printf("ADC: %d \tVoltage: %.2fV\n", adcValue, voltage); // Print ADC value and voltage
    delay(30);                                               // Delay 30 milliseconds
}

// Main function
int main(void) {
    if (setup() == -1) return -1;                            // Call setup function, exit if it returns -1
    while (1) {                                              // Infinite loop
        loop();                                              // Call loop function
    }
    return 0;                                                // Return 0 (never reached)
}
