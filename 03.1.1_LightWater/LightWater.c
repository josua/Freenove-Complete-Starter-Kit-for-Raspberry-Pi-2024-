/*
Filename    : LightWater.c
Description : Use LEDBar Graph(10 LED) 
Author      : Philippe Jos
Modified    : 17/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/03.1.1_LightWater
*/
#include <stdio.h>                                              // Include standard I/O library
#include <stdlib.h>                                             // Include standard library
#include <wiringPi.h>                                           // Include WiringPi library for GPIO control

#define LED_COUNTS 10                         					// Define the number of LEDs
int pins[LED_COUNTS] = {0, 1, 2, 3, 4, 5, 6, 8, 9, 10}; 		// Array to store the pin numbers for LEDs

// Function to initialize the wiringPi library and configure the LED pins
void setup() {
    if (wiringPiSetup() == -1) {              					// Initialize wiringPi and check for setup errors
        printf("Setup wiringPi failed!\n");
        exit(1);                              					// Exit the program if setup fails
    }
    
    for (int i = 0; i < LED_COUNTS; i++) {    					// Loop through the pin array
        pinMode(pins[i], OUTPUT);             					// Set each pin to output mode
        digitalWrite(pins[i], HIGH);          					// Ensure all LEDs are initially off
    }
    printf("Setup complete. LED pins initialized.\n");
}

// Function to run the LED sequence from the first to the last LED
void runLEDSequenceForward() {
    for (int i = 0; i < LED_COUNTS; i++) {    					// Loop through the LED pins
        digitalWrite(pins[i], LOW);           					// Turn on the LED
        delay(100);                           					// Wait for 100 milliseconds
        digitalWrite(pins[i], HIGH);          					// Turn off the LED
    }
}

// Function to run the LED sequence from the last to the first LED
void runLEDSequenceBackward() {
    for (int i = LED_COUNTS - 1; i >= 0; i--) { 				// Loop through the LED pins in reverse order
        digitalWrite(pins[i], LOW);           					// Turn on the LED
        delay(100);                           					// Wait for 100 milliseconds
        digitalWrite(pins[i], HIGH);          					// Turn off the LED
    }
}

// Main loop function to continuously run the LED sequences
void loop() {
    runLEDSequenceForward();                  					// Run the forward LED sequence
    runLEDSequenceBackward();                 					// Run the backward LED sequence
}

// Main function to start the setup and loop functions
int main(void) {
    setup();                                  					// Call the setup function
    while (1) {                               					// Infinite loop
        loop();                               					// Continuously call the loop function
    }
    return 0;                                 					// Return 0 (this line will never be reached)
}

