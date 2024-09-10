/*
Filename    : Alertor.c
Description : Make Alertor with buzzer and button.
Author      : Philippe Jos
Modified    : 06/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/06.2.1_Alertor
*/
#include <stdio.h>                                                       // Include standard I/O library
#include <math.h>                                                        // Include math library for sine calculations
#include <wiringPi.h>                                                    // Include WiringPi library for GPIO control
#include <softTone.h>                                                    // Include WiringPi softTone library
#include <softPwm.h>                                                     // Include WiringPi softPWM library

#define BUZZER_PIN 0                                                     // Define the buzzer pin
#define BUTTON_PIN 1                                                     // Define the button pin
#define LED_PIN 24                                                       // Define the LED pin for toggling and PWM

// Pre-calculate the conversion factor for degrees to radians
const double DEG_TO_RAD = M_PI / 180.0;                                  // Conversion factor for degrees to radians

// Setup function to initialize WiringPi and configure pins
void setup() {
    wiringPiSetup();                                                     // Initialize WiringPi
    pinMode(BUZZER_PIN, OUTPUT);                                         // Set buzzer pin as output
    pinMode(BUTTON_PIN, INPUT);                                          // Set button pin as input
    pinMode(LED_PIN, OUTPUT);                                            // Set LED pin as output
    softToneCreate(BUZZER_PIN);                                          // Create softTone on buzzer pin
    pullUpDnControl(BUTTON_PIN, PUD_UP);                                 // Enable pull-up resistor on button pin
    softPwmCreate(LED_PIN, 0, 100);                                      // Create softPWM on LED pin
}

// Function to handle the alertor functionality
void alertor(int pin) {
    double sinVal, toneVal;                                              // Variables for sine value and tone value
    for (float x = 0; x < 360; x += 0.1) {                               // Loop through 0 to 360 degrees
        if (digitalRead(BUTTON_PIN) == LOW) {                            // If button is pressed
            sinVal = sin(x * DEG_TO_RAD);                                // Calculate the sine value
            toneVal = 500 + sinVal * 175;                                // Add the resonant frequency and weighted sine value
            softToneWrite(pin, toneVal);                                 // Output corresponding PWM
            delay(2);                                                    // Delay for 2 milliseconds
        } else {                                                         // If button is released
            break;                                                       // Exit the loop
        }
    }
}

// Function to stop the alertor
void stopAlertor(int pin) {
    softToneWrite(pin, 0);                                               // Stop the tone on the buzzer pin
}

// Loop function to handle button press and control the LED and buzzer
void loop() {
    if (digitalRead(BUTTON_PIN) == LOW) {                                // If button is pressed
        softPwmWrite(LED_PIN, 100);                                      // Ensure the PWM LED is on
        alertor(BUZZER_PIN);                                             // Turn on buzzer
    } else {                                                             // If button is released
        softPwmWrite(LED_PIN, 0);                                        // Ensure the PWM LED is off
        stopAlertor(BUZZER_PIN);                                         // Turn off buzzer
    }
}

// Main function to set up and repeatedly call the loop function
int main(void) {
    setup();                                                             // Call the setup function to initialize
    while (1) {                                                          // Infinite loop
        loop();                                                          // Call the loop function
    }
    return 0;                                                            // Return 0 (never reached)
}
