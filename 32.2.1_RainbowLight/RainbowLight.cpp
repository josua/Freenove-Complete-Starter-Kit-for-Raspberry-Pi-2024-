/*
Filename    : RainbowLight.cpp
Description : Use freenve 8RGB LED module to achieve rainbow lights.
Author      : Philippe Jos
Modified    : 15/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/32.2.1_RainbowLight
*/
#include <stdio.h>                                              // Include standard I/O library
#include <wiringPi.h>                                           // Include WiringPi library for GPIO control
#include <ADCDevice.hpp>                                        // Include ADCDevice library
#include "Freenove_WS2812_Lib_for_Raspberry_Pi.hpp"             // Include Freenove WS2812 library

// Global pointers for LED and ADC objects
Freenove_WS2812 *leds;
ADCDevice *adc;

// Global variables for RGB values
int red, green, blue;

// Function to convert HSL to RGB
void HSLtoRGB(int degree) {
    degree = degree / 360.0 * 255;
    if (degree < 85) {
        red = 255 - degree * 3;
        green = degree * 3;
        blue = 0;
    } else if (degree < 170) {
        degree = degree - 85;
        red = 0;
        green = 255 - degree * 3;
        blue = degree * 3;
    } else {
        degree = degree - 170;
        red = degree * 3;
        green = 0;
        blue = 255 - degree * 3;
    }
}

// Function to handle the Ctrl+C signal (SIGINT)
void handleSignal(int signal) {
    if (signal == SIGINT) {                                     // If the signal is SIGINT (Ctrl+C)
        printf("\nExiting program...\n");                       // Print exiting message
        if (leds) {                                             // If the leds object is initialized
            leds->clear();                                      // Clear the LEDs
            leds->show();                                       // Ensure the clear command is sent to the LEDs
        }
        delete leds;                                            // Delete the leds object to free memory
        exit(0);                                                // Exit the program
    }
}

// Function to initialize the ADC device
void initADC() {
    adc = new ADCDevice();                                      // Create a new ADCDevice object

    if (adc->detectI2C(0x4b)) {                                 // Detect the ADS7830
        delete adc;                                             // Free previously pointed memory
        adc = new ADS7830(0x4b);                                // Create an instance of ADS7830
    } else if (adc->detectI2C(0x48)) {                          // Detect the PCF8591
        delete adc;                                             // Free previously pointed memory
        adc = new PCF8591();                                    // Create an instance of PCF8591
    } else {
        printf("No correct I2C address found.\n"
               "Please use command 'i2cdetect -y 1' to check the I2C address!\n"
               "Program Exit.\n");
        exit(-1);                                               // Exit the program if no correct I2C address is found
    }
}

// Function to initialize the LED strip
void initLED() {
    leds = new Freenove_WS2812(18, 8, GRB);                     // Initialize with pin, number of LEDs, and type
    leds->set_Led_Brightness(50);                               // Set initial brightness
}

// Setup function to initialize the hardware
void setup() {
    initADC();                                                  // Initialize the ADC device
    initLED();                                                  // Initialize the LED strip
}

// Loop function to continuously update the LED colors
void loop() {
    for (int i = 0; i < 8; i++) {
        int degree = (int)(adc->analogRead(0) / 255.0 * 360 + i * 45); // Read analog value and calculate degree
        if (degree > 360) {
            degree = degree - 360;                              // Adjust degree if it exceeds 360
        }
        HSLtoRGB(degree);                                       // Convert HSL to RGB
        leds->set_Led_Color(i, red, green, blue);               // Set the LED color
        leds->show();                                           // Show the updated color on the LEDs
    }
}

int main() {
    signal(SIGINT, handleSignal);                               // Register the signal handler for SIGINT
    setup();                                                    // Call the setup function
    while (1) {                                                 // Infinite loop
        loop();                                                 // Continuously call the loop function
    }
    return 0;                                                   // Return 0 (this line will never be reached)
}
