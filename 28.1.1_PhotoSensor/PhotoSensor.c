/*
Filename    : PhotoSensor.c
Description : U-shaped photoelectric sensor control LED. 
Author      : Philippe Jos
Modified    : 14/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/28.1.1_PhotoSensor
*/
#include <stdio.h>                                              // Include standard I/O library
#include <cstdlib>                                              // Include standard library for exit()
#include <wiringPi.h>                                           // Include WiringPi library

#define SENSOR_PIN 1                                            // Define the sensor pin (GPIO 18 in wiringPi)
#define LED_PIN 0                                               // Define the LED pin for visual indication (GPIO 17 in wiringPi)

// Function to initialize the GPIO pins
void setup() {
    if (wiringPiSetup() == -1) {                                // Initialize wiringPi and check for setup errors
                printf("Setup wiringPi failed!\n");             // Print error message if setup fails
                exit(1);                                        // Exit program if setup fails
    }
    pinMode(SENSOR_PIN, INPUT);                                 // Set sensor pin to input mode
    pinMode(LED_PIN, OUTPUT);                                   // Set LED pin to output mode
    printf("Setup complete. Sensor pin: %d, LED pin: %d\n",     // Print setup complete message with pin numbers
            SENSOR_PIN, LED_PIN);
}

// Function to read the sensor value
int readSensorValue() {
    return digitalRead(SENSOR_PIN);                             // Read and return the sensor value
}

// Function to update the LED based on the sensor value
void updateLed(int sensorValue) {
    if (sensorValue == 1) {                                     // Check if the sensor is blocked
                digitalWrite(LED_PIN, HIGH);                    // Turn on the LED
                printf("The sensor is blocked, LED turned on\n"); // Print message indicating the LED is turned on
    } else {                                                    // Check if the sensor is not blocked
                digitalWrite(LED_PIN, LOW);                     // Turn off the LED
                printf("The sensor is not blocked, LED turned off\n"); // Print message indicating the LED is turned off
    }
}

// Main loop function to continuously check sensor and control LED
void loop() {
    int sensorValue = readSensorValue();                        // Read the sensor value
    printf("Sensor value: %d\n", sensorValue);                  // Print the sensor value for debugging
    updateLed(sensorValue);                                     // Update the LED based on the sensor value
    delay(250);                                                 // Delay for 250 milliseconds
}

// Main function to call setup and loop functions
int main() {
    setup();                                                    // Call the setup function
    while (1) {                                                 // Infinite loop
        loop();                                                 // Continuously call the loop function
    }
    return 0;                                                   // Return 0 (this line will never be reached)
}
