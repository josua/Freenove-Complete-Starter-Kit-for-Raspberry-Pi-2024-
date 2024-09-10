/*
Filename    : BreathingLED.c
Description : Make breathing LED with PWM
Author      : Philippe Jos
Modified    : 06/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/04.1.1_BreathingLED
*/
#include <stdio.h>                          // Include standard I/O library
#include <wiringPi.h>                       // Include WiringPi library for GPIO control
#include <softPwm.h>                        // Include soft PWM library

#define LED_PIN 1 // Define the LED pin

void setup() {
    wiringPiSetup();                        // Initialize wiringPi
    softPwmCreate(LED_PIN, 0, 100);         // Create a software PWM pin
}

void loop() {
    for (int i = 0; i < 100; i++) {         // Make the LED brighter
        softPwmWrite(LED_PIN, i);           // Set PWM duty cycle
        delay(20);                          // Wait for 20 milliseconds
    }
    delay(300);                             // Wait for 300 milliseconds

    for (int i = 100; i >= 0; i--) {        // Make the LED dimmer
        softPwmWrite(LED_PIN, i);           // Set PWM duty cycle
        delay(20);                          // Wait for 20 milliseconds
    }
    delay(300);                             // Wait for 300 milliseconds
}

int main(void) {
    setup();                                // Call setup to initialize
    while (1) {                             // Infinite loop
        loop();                             // Call loop function
    }
    return 0;                               // This line will never be reached
}

