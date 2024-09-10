/*
Filename    : BreathingButtonLED.c
Description : Make breathing LED with PWM
Author      : Philippe Jos
Modified    : 06/07/2024
*/
#include <stdio.h>                                                          // Include standard I/O library
#include <stdlib.h>                                                         // Include standard library
#include <errno.h>                                                          // Include error number definitions
#include <string.h>                                                         // Include string operations for strerror
#include <wiringPi.h>                                                       // Include WiringPi library
#include <softPwm.h>                                                        // Include soft PWM library

#define LED_PIN 0                                                           // Define the LED pin for toggling and PWM
#define BUTTON_PIN 1                                                        // Define the button pin
#define DEBOUNCE_TIME 200                                                   // Debounce time in milliseconds

volatile int ledState = LOW;                                                // Store the state of the LED (initially off)
volatile unsigned long lastInterruptTime = 0;                               // Store the last interrupt time for debouncing

// Interrupt Service Routine (ISR) to handle button press
void handleButtonPress() {
    unsigned long interruptTime = millis();                                 // Get the current time in milliseconds

    // If interrupts come faster than debounceTime, assume it's a bounce and ignore
    if (interruptTime - lastInterruptTime > DEBOUNCE_TIME) {
        ledState = !ledState;                                               // Toggle the LED state
        if (ledState) {
            printf("LED turned on\n");
        } else {
            printf("LED turned off\n");
            softPwmWrite(LED_PIN, 0);                                       // Ensure the PWM LED is off
        }
    }
    lastInterruptTime = interruptTime;                                      // Update the interrupt time
}

void setup() {
    printf("Program is starting...\n");                                     // Print starting message
    wiringPiSetup();                                                        // Initialize WiringPi
    pinMode(BUTTON_PIN, INPUT);                                             // Set button pin to input
    pullUpDnControl(BUTTON_PIN, PUD_UP);                                    // Enable pull-up resistor on button pin
    softPwmCreate(LED_PIN, 0, 100);                                         // Create SoftPWM pin for LED

    // Setup ISR for button pin
    if (wiringPiISR(BUTTON_PIN, INT_EDGE_FALLING, &handleButtonPress) < 0) {
        fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));      // Print error message if ISR setup fails
        exit(1);                                                            // Exit program if error
    }
}

// Function to handle the LED breathing effect
void handleLEDBreathing() {
    for (int i = 0; i < 100; i++) {                                         // Make the LED brighter
        if (!ledState) return;                                              // Exit if LED state changed
        softPwmWrite(LED_PIN, i);                                           // Set PWM value for LED
        delay(10);                                                          // Delay for smooth transition
    }
    delay(100);                                                             // Delay before dimming
    for (int i = 100; i >= 0; i--) {                                        // Make the LED darker
        if (!ledState) return;                                              // Exit if LED state changed
        softPwmWrite(LED_PIN, i);                                           // Set PWM value for LED
        delay(10);                                                          // Delay for smooth transition
    }
    delay(100);                                                             // Delay before next cycle
}

void loop() {
    if (ledState) {                                                         // If the LED state is on, handle the breathing effect
        handleLEDBreathing();                                               // Call function to handle LED breathing effect
    } else {                                                                // If the LED state is off
        softPwmWrite(LED_PIN, 0);                                           // Ensure the PWM LED is off
        delay(50);                                                          // Small delay to reduce CPU usage
    }
}

int main(void) {
    setup();                                                                // Call the setup function to initialize
    while (1) {                                                             // Infinite loop
        loop();                                                             // Call the loop function
    }
    return 0;                                                               // This line will never be reached
}

