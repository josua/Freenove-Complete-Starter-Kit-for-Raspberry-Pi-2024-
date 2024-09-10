/*
Filename    : Ledpixel.cpp
Description : Use freenove 8RGB LED module to achieve a flowing light.
Author      : Philippe Jos
Modified    : 15/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/32.1.1_Ledpixel
*/
#include <signal.h>                                         // Include signal handling library
#include <wiringPi.h>                                       // Include WiringPi library for GPIO control
#include "Freenove_WS2812_Lib_for_Raspberry_Pi.hpp"         // Include Freenove WS2812 library

// Global pointer to Freenove_WS2812 object
Freenove_WS2812 *leds;

// Function to constrain a value between a minimum and maximum range
int constrain(int value, int min, int max) {
    if (value > max) {                                      // If value is greater than max
        return max;                                         // Return max
    } else if (value < min) {                               // If value is less than min
        return min;                                         // Return min
    } else {                                                // If value is within the range
        return value;                                       // Return the value
    }
}

// Function to handle the Ctrl+C signal (SIGINT)
void handleSignal(int signal) {
    if (signal == SIGINT) {                                 // If the signal is SIGINT (Ctrl+C)
        printf("\nExiting program...\n");                   // Print exiting message
        if (leds) {                                         // If the leds object is initialized
            leds->clear();                                  // Clear the LEDs
            leds->show();                                   // Ensure the clear command is sent to the LEDs
        }
        delete leds;                                        // Delete the leds object to free memory
        exit(0);                                            // Exit the program
    }
}

// Setup function to initialize the LED pixel object
void setup() {
    printf("Initializing LED pixel...\n");                  // Print initializing message
    leds = new Freenove_WS2812(18, 8, GRB);                 // Initialize with pin, number of LEDs, and type
    if (!leds) {                                            // If initialization failed
        printf("Failed to initialize LED pixel\n");         // Print failure message
        exit(1);                                            // Exit the program with error
    }
    leds->set_Led_Brightness(50);                           // Set initial brightness
    printf("Setup completed\n");                            // Print setup completed message
}

// Function to cycle through LED colors
void cycleColors(unsigned int red, unsigned int green, unsigned int blue, int delayTime) {
    for (int i = 0; i < 8; i++) {                           // Loop through each LED
        leds->set_Led_Color(i, red, green, blue);           // Set color for current LED
        leds->show();                                       // Update the LED display
        delay(delayTime);                                   // Wait for the specified delay time
    }
}

// Loop function to update the LED colors
void loop() {
    cycleColors(255, 0, 0, 100);                            // Cycle through red color
    cycleColors(0, 255, 0, 100);                            // Cycle through green color
    cycleColors(0, 0, 255, 100);                            // Cycle through blue color
    cycleColors(255, 255, 0, 100);                          // Cycle through yellow color
    cycleColors(255, 0, 255, 100);                          // Cycle through magenta color
    cycleColors(0, 255, 255, 100);                          // Cycle through cyan color
}

int main(void) {
    signal(SIGINT, handleSignal);                           // Register the signal handler for SIGINT

    setup();                                                // Call the setup function
    while (1) {                                             // Infinite loop
        loop();                                             // Continuously call the loop function
    }
    return 0;                                               // Return 0 (this line will never be reached)
}

