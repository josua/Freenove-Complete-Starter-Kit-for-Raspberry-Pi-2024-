/*
Filename    : TouchSensor.c
Description : Touch Sensor TTP223 Control LED brightness. 
Author      : Philippe Jos
Modified    : 14/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/27.1.1_TouchSensor
*/
#include <stdio.h>                                  // Include standard I/O library
#include <cstdlib>                                  // Include standard library for exit()
#include <wiringPi.h>                               // Include WiringPi library
#include <softPwm.h>                                // Include soft PWM library

#define LED_PIN    0                                // Define the LED_PIN (GPIO 17)
#define SENSOR_PIN 1                                // Define the SENSOR_PIN (GPIO 18)

int sensorState = LOW;                              // Store the state of the sensor
int lastSensorState = LOW;                          // Store the last state of the sensor
long lastChangeTime = 0;                            // Store the change time of sensor state
long captureTime = 10;                              // Set the debounce time for sensor state (in milliseconds)
int sensorValue;
int grade = 0;                                      // Track the number of sensor triggers
int lastGrade = -1;                                 // Track the last grade for LED brightness

void setup() {
    if (wiringPiSetup() == -1) {                    // Initialize wiringPi library and check for setup errors
        printf("Setup wiringPi failed!\n");
        exit(1);
    }
    softPwmCreate(LED_PIN, 0, 100);                 // Create a software PWM on the LED pin with range 0-100
    pinMode(SENSOR_PIN, INPUT);                     // Set the sensor pin as input
    pullUpDnControl(SENSOR_PIN, PUD_DOWN);          // Set pull-down resistor on the sensor pin
    printf("Setup complete. LED pin: %d, Sensor pin: %d\n", LED_PIN, SENSOR_PIN);
}

void readSensorValue() {
    sensorValue = digitalRead(SENSOR_PIN);          // Read the sensor value
    if (sensorValue != lastSensorState) {           // Check if the sensor value has changed
        lastChangeTime = millis();                  // Update the last change time
    }
}

void updateSensorState() {
    if ((millis() - lastChangeTime) > captureTime) {// Check if the state is stable
        if (sensorValue != sensorState) {           // Check if the stable state is different from the current state
            sensorState = sensorValue;              // Update the sensor state
            if (sensorState == HIGH) {              // If the sensor is triggered
                grade = (grade + 1) % 5;            // Increment the grade and wrap around after 4
            }
        }
    }
}

void updateLedBrightness() {
    if (grade != lastGrade) {                       // Only update and print if the grade has changed
        switch (grade) {
            case 1:
                softPwmWrite(LED_PIN, 25);          // Set LED to 25% brightness
                printf("LED Brightness: 25%\n");
                break;
            case 2:
                softPwmWrite(LED_PIN, 50);          // Set LED to 50% brightness
                printf("LED Brightness: 50%\n");
                break;
            case 3:
                softPwmWrite(LED_PIN, 75);          // Set LED to 75% brightness
                printf("LED Brightness: 75%\n");
                break;
            case 4:
                softPwmWrite(LED_PIN, 100);         // Set LED to 100% brightness
                printf("LED Brightness: 100%\n");
                break;
            default:
                softPwmWrite(LED_PIN, 0);           // Turn off the LED
                printf("LED Brightness: 0%\n");
                break;
        }
        lastGrade = grade;                          // Update the last grade to the current grade
    }
}

void loop() {
    readSensorValue();                              // Read the sensor value
    updateSensorState();                            // Update the sensor state based on debouncing
    updateLedBrightness();                          // Update the LED brightness based on the grade
    lastSensorState = sensorValue;                  // Update the last sensor state
}

int main(void) {
    setup();                                        // Call the setup function
    while (1) {                                     // Infinite loop
        loop();                                     // Continuously call the loop function
    }
    return 0;                                       // Return 0 (this line will never be reached)
}
