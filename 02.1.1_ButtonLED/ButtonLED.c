/*
Filename    : ButtonLED.c
Description : Control led by button
Author      : Philippe Jos
Modified    : 17/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/02.1.1_ButtonLED
*/
#include <stdio.h>                            // Include standard I/O library
#include <wiringPi.h>                         // Include WiringPi library for GPIO control

#define LED_PIN    0                          // Define the LED pin
#define BUTTON_PIN 1                          // Define the button pin

void setup() {
    wiringPiSetup();                          // Initialize wiringPi
    pinMode(LED_PIN, OUTPUT);                 // Set LED_PIN to output
    pinMode(BUTTON_PIN, INPUT);               // Set BUTTON_PIN to input
    pullUpDnControl(BUTTON_PIN, PUD_UP);      // Enable pull-up resistor on BUTTON_PIN
}

void loop() {
    if (digitalRead(BUTTON_PIN) == LOW) {     // If button is pressed (LOW state)
        digitalWrite(LED_PIN, HIGH);          // Turn on LED
    } else {                                  // If button is released (HIGH state)
        digitalWrite(LED_PIN, LOW);           // Turn off LED
    }
}

int main(void) {
    setup();                                  // Call setup function to initialize
    while (1) {                               // Infinite loop to continuously check button state
        loop();                               // Call loop function to check button and control LED
    }
    return 0;                                 // Return 0 (never reached)
}


