/*
Filename    : SenseLED.c
Description : Control led with infrared Motion sensor
Author      : Philippe Jos
Modified    : 13/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/23.1.1_SenseLED
*/
#include <wiringPi.h>                                         // Include WiringPi library
#include <stdio.h>                                            // Include standard I/O library

#define LED_PIN    1  	// Define the LED pin (GPIO 18 in WiringPi numbering)
#define SENSOR_PIN 0	// Define the sensor pin (GPIO 17 in WiringPi numbering)

// Setup function to initialize the GPIO pins
void setup() {
    wiringPiSetup();                                          // Initialize the wiringPi library
    pinMode(LED_PIN, OUTPUT);                                 // Set the LED pin as an output
    pinMode(SENSOR_PIN, INPUT);                               // Set the sensor pin as an input
}

// Loop function to continuously check the sensor and control the LED
void loop() {		
    if (digitalRead(SENSOR_PIN) == HIGH) {                    // If the sensor pin reads HIGH
        digitalWrite(LED_PIN, HIGH);                          // Turn on the LED
        printf("Detected\n");                                 // Print message to console
    } else {				
        digitalWrite(LED_PIN, LOW);                           // Turn off the LED
        printf("\n");                                         // Print message to console
    }
    delay(500);                                               // Add a small delay to reduce the rate of printing messages
}

// Main function to call setup and loop functions
int main() {
    setup();                                                  // Call the setup function to initialize the GPIO pins
    while (1) {
        loop();                                               // Continuously call the loop function
    }
    return 0;                                                 // This line will never be reached
}

