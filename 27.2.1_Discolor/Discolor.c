/*
Filename    : Discolor.c
Description : Touch Sensor TTP223 control RGB LED. 
Author      : Philippe Jos
Modified    : 14/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/27.2.1_Discolor
*/
#include <stdio.h>                                          // Include standard I/O library
#include <stdlib.h>                                         // Include standard library for exit()
#include <wiringPi.h>                                       // Include WiringPi library
#include <softPwm.h>                                        // Include soft PWM library

#define SENSOR_PIN 1                                        // Define the sensor pin
#define LED_PIN_RED 0                                       // Define the red LED pin
#define LED_PIN_GREEN 2                                     // Define the green LED pin
#define LED_PIN_BLUE 3                                      // Define the blue LED pin

int sensorState = LOW;                                      // Store the state of the sensor
int lastSensorState = LOW;                                  // Store the last state of the sensor
long lastChangeTime = 0;                                    // Store the change time of sensor state
const long captureTime = 10;                                // Set the debounce time for sensor state (in milliseconds)
int sensorValue;                                            // Variable to store the sensor value
int grade = 0;                                              // Track the number of sensor triggers
int lastGrade = -1;                                         // Track the last grade for LED color

// Function to set up LED pins for PWM
void setupLedPin(void) {
    softPwmCreate(LED_PIN_RED, 0, 100);
    softPwmCreate(LED_PIN_GREEN, 0, 100);
    softPwmCreate(LED_PIN_BLUE, 0, 100);
}

// Function to set the LED color
void setLedColor(int r, int g, int b) {
    softPwmWrite(LED_PIN_RED, 100 - r);                     // Invert the red value
    softPwmWrite(LED_PIN_GREEN, 100 - g);                   // Invert the green value
    softPwmWrite(LED_PIN_BLUE, 100 - b);                    // Invert the blue value
}

// Setup function to initialize the GPIO pins
void setup() {
    if (wiringPiSetup() == -1) {                            // Initialize WiringPi library and check for setup errors
        printf("Setup wiringPi failed!\n");
        exit(1);
    }
    pinMode(SENSOR_PIN, INPUT);                             // Set SENSOR_PIN to input
    pullUpDnControl(SENSOR_PIN, PUD_DOWN);                  // Pull down to low level
    setupLedPin();                                          // Setup LED pins for PWM
    setLedColor(0, 0, 0);                                   // Set initial LED color to black
    printf("Setup complete. Sensor pin: %d, LED pins: %d, %d, %d\n", 
           SENSOR_PIN, LED_PIN_RED, LED_PIN_GREEN, LED_PIN_BLUE);
}

// Function to read the sensor value
void readSensorValue() {
    sensorValue = digitalRead(SENSOR_PIN);                  // Read the sensor value
    if (sensorValue != lastSensorState) {                   // Check if the sensor value has changed
        lastChangeTime = millis();                          // Update the last change time
    }
}

// Function to update the sensor state based on debouncing
void updateSensorState() {
    if ((millis() - lastChangeTime) > captureTime) {        // Check if the state is stable
        if (sensorValue != sensorState) {                   // Check if the stable state is different from the current state
            sensorState = sensorValue;                      // Update the sensor state
            if (sensorState == HIGH) {                      // If the sensor is triggered
                grade = (grade + 1) % 8;                    // Increment the grade and wrap around after 7
                printf("Sensor is pressed!\n");
            }
        }
    }
}

// Function to update the LED color based on the grade
void updateLedColor() {
    if (grade != lastGrade) {                               // Only update and print if the grade has changed
        int r, g, b;
        switch (grade) {
            case 1:
                r = 100; g = 0; b = 0;                      // Set color to red
                printf("The current color is red\n");
                break;
            case 2:
                r = 0; g = 100; b = 0;                      // Set color to green
                printf("The current color is green\n");
                break;
            case 3:
                r = 0; g = 0; b = 100;                      // Set color to blue
                printf("The current color is blue\n");
                break;
            case 4:
                r = 100; g = 100; b = 0;                    // Set color to yellow
                printf("The current color is yellow\n");
                break;
            case 5:
                r = 100; g = 0; b = 100;                    // Set color to magenta
                printf("The current color is magenta\n");
                break;
            case 6:
                r = 0; g = 100; b = 100;                    // Set color to cyan
                printf("The current color is cyan\n");
                break;
            case 7:
                r = 0; g = 0; b = 0;                        // Set color to black
                printf("The current color is black\n");
                break;
            default:
                grade = 0;                                  // Reset the grade
                r = 100; g = 100; b = 100;                  // Default color white
                printf("The current color is white\n");
                break;
        }
        setLedColor(r, g, b);                               // Set the LED color
        lastGrade = grade;                                  // Update the last grade to the current grade
    }
}

// Loop function to read sensor value, update sensor state, and update LED color
void loop() {
    readSensorValue();                                      // Read the sensor value
    updateSensorState();                                    // Update the sensor state based on debouncing
    updateLedColor();                                       // Update the LED color based on the grade
    lastSensorState = sensorValue;                          // Update the last sensor state
}

// Main function to call setup and loop functions
int main(void) {
    setup();                                                // Call the setup function
    while (1) {                                             // Infinite loop
        loop();                                             // Continuously call the loop function
    }
    return 0;                                               // Return 0 (this line will never be reached)
}
