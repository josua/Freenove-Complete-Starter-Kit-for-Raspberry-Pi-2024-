/*
Filename    : Alertor.c
Description : Make a sound and light alarm with a buzzer and u type photoelectric sensor. 
Author      : Philippe Jos
Modified    : 14/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/28.2.1_Alertor
*/
#include <stdio.h>                                          // Include the standard I/O library for printing messages
#include <stdlib.h>                                         // Include the standard library for exit() function
#include <stdbool.h>                                        // Include the standard boolean library
#include <wiringPi.h>                                       // Include the wiringPi library for GPIO control

#define LED_PIN    0                                        // Define the LED pin (GPIO 17 in wiringPi)
#define SENSOR_PIN 1                                        // Define the sensor pin (GPIO 18 in wiringPi)
#define BUZZER_PIN 2                                        // Define the buzzer pin (GPIO 27 in wiringPi)

volatile bool sensorTriggered = false;                      // Volatile variable to track sensor state
volatile int sensorEventType = 0;                           // 1 for rising edge, -1 for falling edge
long debounceTime = 200;                                    // Debounce time in milliseconds
long lastInterruptTime = 0;                                 // Last interrupt time

// Function to trigger the alarm (LED and buzzer) a specified number of times
void alarm(int times) {
    for (int i = 0; i < times; ++i) {
        digitalWrite(BUZZER_PIN, HIGH);                     // Turn on the buzzer
        digitalWrite(LED_PIN, HIGH);                        // Turn on the LED
        printf("Alarm on\n");
        delay(50);                                          // Wait for 50 milliseconds
        digitalWrite(BUZZER_PIN, LOW);                      // Turn off the buzzer
        digitalWrite(LED_PIN, LOW);                         // Turn off the LED
        printf("Alarm off\n");
        delay(50);                                          // Wait for 50 milliseconds
    }
}

// ISR (Interrupt Service Routine) to handle sensor events
void sensorEvent(void) {
    long interruptTime = millis();                          // Get the current time

    // If interrupts come faster than debounceTime, assume it's a bounce and ignore
    if (interruptTime - lastInterruptTime > debounceTime) {
        sensorTriggered = true;                             // Set the sensor triggered flag
        if (digitalRead(SENSOR_PIN) == HIGH) {
            sensorEventType = 1;                            // Blocked event detected
            printf("Blocked event detected!\n");
        } else {
            sensorEventType = -1;                           // Released event detected
            printf("Released event detected!\n");
        }
    }
    lastInterruptTime = interruptTime;                      // Update the last interrupt time
}

// Setup function to initialize the GPIO pins and configure the interrupt
void setup() {
    if (wiringPiSetup() == -1) {                            // Initialize wiringPi and check for setup errors
        printf("Setup wiringPi failed!\n");
        exit(1);
    }
    pinMode(LED_PIN, OUTPUT);                               // Set LED pin to output mode
    pinMode(SENSOR_PIN, INPUT);                             // Set sensor pin to input mode
    pinMode(BUZZER_PIN, OUTPUT);                            // Set buzzer pin to output mode
    pullUpDnControl(SENSOR_PIN, PUD_DOWN);                  // Ensure the pull-down resistor is enabled
    if (wiringPiISR(SENSOR_PIN, INT_EDGE_BOTH, &sensorEvent) < 0) {  // Configure ISR for sensor pin
        printf("Unable to setup ISR\n");
        exit(1);
    }
    digitalWrite(BUZZER_PIN, LOW);                          // Ensure the buzzer is off initially
    printf("Setup complete. Waiting for sensor events...\n");
}

// Main loop function to handle sensor-triggered events
void loop() {
    if (sensorTriggered) {                                  // Check if the sensor has been triggered
        alarm(3);                                           // Trigger the alarm 3 times
        sensorTriggered = false;                            // Reset the sensor triggered flag
    }
    delay(100);                                             // Small delay to reduce CPU usage
}

// Main function to start the setup and loop functions
int main(void) {
    setup();                                                // Call the setup function
    while (1) {                                             // Infinite loop
        loop();                                             // Continuously call the loop function
    }
    return 0;                                               // Return 0 (this line will never be reached)
}

