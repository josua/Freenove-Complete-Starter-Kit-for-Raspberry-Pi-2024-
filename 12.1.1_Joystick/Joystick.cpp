/*
Filename    : Joystick.cpp
Description : Read joystick controller input x, y, z axis values
Author      : Philippe Jos
Modified    : 09/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/12.1.1_Joystick
*/

#include <stdio.h>                              // Include standard I/O library
#include <cstdlib>                              // Include standard library for exit()
#include <memory>                               // Include memory management library
#include <wiringPi.h>                           // Include WiringPi library for GPIO
#include <softPwm.h>                            // Include software PWM library
#include <ADCDevice.hpp>                        // Include ADC device header

// Define pin for z axis
#define Z_PIN 1                                 // Define Z_PIN as pin 1

// Sensor I2C addresses
constexpr int PCF8591_ADDRESS = 0x48;           // I2C address for PCF8591
constexpr int ADS7830_ADDRESS = 0x4b;           // I2C address for ADS7830

// ADC analog read channels
constexpr int X_CHANNEL = 1;                    // Define X_CHANNEL as channel 1
constexpr int Y_CHANNEL = 0;                    // Define Y_CHANNEL as channel 0

// Store an instance of an ADC Device object, as a smart pointer
std::unique_ptr<ADCDevice> adc;                 // Unique pointer to ADCDevice object

void initADC() {                                // Function to initialize ADC
    if (adc->detectI2C(PCF8591_ADDRESS)) {      // Check for PCF8591 I2C address
        adc = std::make_unique<PCF8591>();      // Create PCF8591 instance if detected
    } else if (adc->detectI2C(ADS7830_ADDRESS)) { // Check for ADS7830 I2C address
        adc = std::make_unique<ADS7830>();      // Create ADS7830 instance if detected
    } else {                                    // If no ADC detected
        printf("No correct I2C address found,\n"
               "Please use command 'i2cdetect -y 1' to check the I2C address!\n"
               "Program Exit.\n");                          // Print error message
        exit(EXIT_FAILURE);                     // Exit program if no ADC detected
    }
}

void setup() {                                  // Function to set up the program
    adc = std::make_unique<ADCDevice>();        // Create initial ADCDevice instance
    initADC();                                  // Initialize ADC

    if (wiringPiSetup() == -1) {                // Set up WiringPi and check for failure
        fprintf(stderr, "WiringPi setup failed.\n");
        exit(EXIT_FAILURE);                     // Exit program if WiringPi setup fails
    }

    pinMode(Z_PIN, INPUT);                      // Set Z_PIN as an input pin
    pullUpDnControl(Z_PIN, PUD_UP);             // Enable pull-up resistor on Z_PIN
}

void loop() {                                   // Function to read and print joystick controller values
    int x = adc->analogRead(X_CHANNEL);         // Read analog value from X_CHANNEL
    int y = adc->analogRead(Y_CHANNEL);         // Read analog value from Y_CHANNEL
    int z = digitalRead(Z_PIN);                 // Read digital value from Z_PIN
    printf("x: %d \ty: %d \tz: %d \n", x, y, z); // Print x, y, and z axis values
    delay(100);                                 // Delay in milliseconds
}

int main(void) {                                // Main function
    setup();                                    // Call setup function
    while (true) {                              // Infinite loop
        loop();                                 // Call loop function
    }
    return 0;                                   // Return 0 (never reached)
}


