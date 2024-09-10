/*
Filename    : VoiceLamp.c
Description : Make sound control lamp with high-sensitivity microphone sensor.  
Author      : Philippe Jos
Modified    : 14/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/26.1.1_VoiceLamp
*/
#include <stdio.h>                                      // Include standard I/O library
#include <stdlib.h>                                     // Include standard library for general functions
#include <string.h>                                     // Include string library for string manipulation
#include <wiringPi.h>                                   // Include wiringPi library

#define LED_PIN     0                                   // Define the LED pin (GPIO 17 in wiringPi)
#define SENSOR_PIN  1                                   // Define the sensor pin (GPIO 18 in wiringPi)

bool ledState = LOW;                                    // Track the current state of the LED
char waitingAnimation[31];                              // String to store the waiting animation (30 dots max)
int animationIndex = 0;                                 // Current index of the animation
bool increasing = true;                                 // Direction of the animation

void setup() {
    printf("Program is starting ... \n");
    if (wiringPiSetup() == -1) {                        // Initialize wiringPi library and check for setup errors
        printf("Setup wiringPi failed!\n");
        exit(1);
    }
    pinMode(LED_PIN, OUTPUT);                           // Set LED pin to output mode
    pinMode(SENSOR_PIN, INPUT);                         // Set sensor pin to input mode
    printf("Setup complete. LED pin: %d, Sensor pin: %d\n", LED_PIN, SENSOR_PIN);

    waitingAnimation[0] = '\0';                         // Initialize the waiting animation string
}

void loop() {
    int sensorValue = digitalRead(SENSOR_PIN);          // Read the sensor value

    if (sensorValue == HIGH && !ledState) {             // Check if sensor is triggered and LED is currently off
        printf("\nSensor: On\n");                       // Print the sensor state for debugging
        digitalWrite(LED_PIN, HIGH);                    // Turn on the LED
        ledState = HIGH;                                // Update the LED state
        delay(100);                                     // Delay for 100 milliseconds
        digitalWrite(LED_PIN, LOW);                     // Turn off the LED
        ledState = LOW;                                 // Update the LED state
        waitingAnimation[0] = '\0';                     // Reset the waiting animation string
        animationIndex = 0;                             // Reset the animation index
        increasing = true;                              // Reset the animation direction
        printf("\r                             \r");    // Clear the line after sensor trigger
        fflush(stdout);                                 // Ensure the output is printed immediately
    } else if (sensorValue == LOW) {                    // Sensor is not triggered
        if (increasing) {                               // If increasing the number of dots
            waitingAnimation[animationIndex] = '.';     // Append a dot
            waitingAnimation[animationIndex + 1] = '\0';// Null-terminate the string
            animationIndex++;                           // Increment the animation index
            if (animationIndex >= 30) {                 // If maximum dots reached
                increasing = false;                     // Start decreasing
            }
            delay(10);                                  // Small delay for animation timing
        } else {                                        // If decreasing the number of dots
            waitingAnimation[animationIndex] = '\0';    // Null-terminate the string to remove a dot
            animationIndex--;                           // Decrement the animation index
            if (animationIndex <= 0) {                  // If minimum dots reached
                increasing = true;                      // Start increasing
            }
            delay(10);                                  // Small delay for animation timing
        }
        printf("\r%s", waitingAnimation);               // Print the waiting animation string
        fflush(stdout);                                 // Ensure the output is printed immediately
    }
}

int main(void) {
    setup();                                            // Call the setup function
    while (1) {                                         // Infinite loop
        loop();                                         // Continuously call the loop function
    }
    return 0;                                           // Return 0 (this line will never be reached)
}




