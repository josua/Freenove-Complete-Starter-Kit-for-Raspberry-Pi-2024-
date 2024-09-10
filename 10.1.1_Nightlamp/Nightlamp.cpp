/*
Filename    : Nightlamp.cpp
Description : Photoresistor controlling LED
Author      : Philippe Jos
Modified    : 07/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/10.1.1_Nightlamp
*/
#include <stdio.h>                                                          // Include standard I/O library
#include <memory>                                                           // Include memory management for smart pointers
#include <wiringPi.h>                                                       // Include WiringPi library
#include <softPwm.h>                                                        // Include software PWM library
#include <ADCDevice.hpp>                                                    // Include ADC device class

#define LED_PIN     0  // Define the pin for the LED
#define THRESHOLD  10  // Threshold value to handle noise

// Pre-computed ratios
const float PWM_RATIO = 100.0 / 255.0;                                      // Define ratio to map ADC value to PWM
const float VOLTAGE_RATIO = 3.3 / 255.0;                                    // Define ratio to map ADC value to voltage

std::unique_ptr<ADCDevice> adc;                                             // Use smart pointer for automatic memory management

// Setup function for initializing ADC device and PWM pins
int setup() {
    adc = std::make_unique<ADCDevice>();                                    // Initialize ADC device

    if (adc->detectI2C(0x48)) {                                             // Detect the PCF8591 Module
        adc = std::make_unique<PCF8591>();                                  // Create an instance of PCF8591 if detected
    } else if (adc->detectI2C(0x4b)) {                                      // Detect the ADS7830 Module
        adc = std::make_unique<ADS7830>();                                  // Create an instance of ADS7830 if detected
    } else {
        printf("No correct I2C address found,\n"
               "Please use command 'i2cdetect -y 1' to check the I2C address!\n"
               "Program Exit.\n");                          // Print error message
        return -1;                                                          // Exit setup if no valid I2C address is found
    }

    if (wiringPiSetup() == -1) {                                            // Initialize wiringPi
        printf("WiringPi setup failed! \n");
        return -1;                                                          // Exit setup if wiringPi setup fails
    }

    if (softPwmCreate(LED_PIN, 0, 100) != 0) {                              // Create PWM pin for the LED
        printf("PWM setup failed! \n");
        return -1;                                                          // Exit setup if PWM setup fails
    }

    return 0;                                                               // Return 0 if setup is successful
}

// Loop function to read analog value and map it to PWM value for LED
void loop() {
    int value = adc->analogRead(0);                                          // Read analog value of A0 pin
    int pwmValue = (value < THRESHOLD) ? 0 : static_cast<int>(value * PWM_RATIO); // Map ADC value to PWM value

    softPwmWrite(LED_PIN, pwmValue);                                         // Write PWM value to LED pin
    printf("ADC: %d   \tVoltage: %.2fV\n", 
           value, static_cast<float>(value) * VOLTAGE_RATIO);                // Print ADC and voltage values
    delay(10);                                                               // Delay to control loop speed
}

// Main function
int main(void) {
    if (setup() == -1) return -1;                                             // Check if setup was successful
    while (true) loop();                                                      // Loop indefinitely
    return 0;                                                                 // Return 0 (never reached)
}

