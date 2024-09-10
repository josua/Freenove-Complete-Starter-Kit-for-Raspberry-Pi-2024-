/*
Filename    : Blink.c
Description : Basic usage of GPIO. Let led blink.
Author      : Philippe Jos
Modified    : 17/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/01.1.1_Blink
*/
#include <stdio.h>                      // Include standard I/O library
#include <wiringPi.h>                   // Include WiringPi library for GPIO control

#define LED_PIN 0  // Define the pin number for the LED

// Setup function to initialize the wiringPi and pin mode
void setup() {
    wiringPiSetup();                    // Initialize wiringPi library
    pinMode(LED_PIN, OUTPUT);           // Set the LED pin as output
    printf("Using pin %d\n", LED_PIN);  // Print the pin number being used
}

// Loop function to blink the LED
void loop() {
    digitalWrite(LED_PIN, HIGH);        // Turn the LED on
    printf("LED turned on\n");          // Print message indicating LED is on
    delay(1000);                        // Wait for 1 second
    digitalWrite(LED_PIN, LOW);         // Turn the LED off
    printf("LED turned off\n");         // Print message indicating LED is off
    delay(1000);                        // Wait for 1 second
}

// Main function
int main(void) {
    setup();                            // Call setup function to initialize
    while (1) {                         // Infinite loop to continuously blink LED
        loop();                         // Call loop function to blink LED
    }
    return 0;                           // Return 0 (never reached)
}
