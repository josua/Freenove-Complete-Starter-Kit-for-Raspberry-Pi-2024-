/*
Filename    : AlertorBreathingLED.c
Description : Make Alertor with buzzer, button and LED.
Author      : Philippe Jos
Modified    : 06/07/2024
*/
#include <stdio.h>                                                          // Include standard I/O library
#include <math.h>                                                           // Include math library
#include <wiringPi.h>                                                       // Include WiringPi library
#include <softTone.h>                                                       // Include soft tone library
#include <softPwm.h>                                                        // Include soft PWM library

#define BUZZER_PIN  0   // Define the buzzer pin
#define BUTTON_PIN  1   // Define the button pin
#define LED_PIN     24  // Define the LED pin for toggling and PWM

// Pre-calculate the conversion factor for degrees to radians
const double DEG_TO_RAD = M_PI / 180.0;

void setup() {
    wiringPiSetup();                                                        // Initialize WiringPi
    pinMode(BUZZER_PIN, OUTPUT);                                            // Set buzzer pin to output
    pinMode(BUTTON_PIN, INPUT);                                             // Set button pin to input
    pinMode(LED_PIN, OUTPUT);                                               // Set LED pin to output
    softToneCreate(BUZZER_PIN);                                             // Set up buzzer pin for soft tone
    pullUpDnControl(BUTTON_PIN, PUD_UP);                                    // Pull up button pin to HIGH level
    softPwmCreate(LED_PIN, 0, 100);                                         // Create SoftPWM pin for LED
}

void handleBreathingAndAlertor(int pin) {
    double sinVal, toneVal;                                                 // Variables for sine and tone values
    int brightness;                                                         // Variable for LED brightness

    while (digitalRead(BUTTON_PIN) == LOW) {                                // While button is pressed
        for (int i = 0; i < 360; i++) {                                     // Loop through 0 to 360 degrees
            if (digitalRead(BUTTON_PIN) == HIGH) {                          // Exit if button is released
                softPwmWrite(LED_PIN, 0);                                   // Ensure the LED is off
                softToneWrite(pin, 0);                                      // Ensure the buzzer is off
                return;                                                     // Exit the function
            }

            sinVal = sin(i * DEG_TO_RAD);                                   // Calculate the sine value
            toneVal = 500 + sinVal * 175;                                   // Add the resonant frequency and weighted sine value 
            brightness = (int)((sinVal + 1) * 50);                          // Normalize sine value to 0-100 for LED brightness

            softToneWrite(pin, toneVal);                                    // Output corresponding PWM for buzzer
            softPwmWrite(LED_PIN, brightness);                              // Output corresponding PWM for LED

            delay(10);                                                      // Delay to control breathing speed
        }
    }
}

void stopAlertorAndLED(int pin) {
    softToneWrite(pin, 0);                                                  // Ensure the buzzer is off
    softPwmWrite(LED_PIN, 0);                                               // Ensure the LED is off
}

void loop() {
    if (digitalRead(BUTTON_PIN) == LOW) {                                   // If button is pressed
        handleBreathingAndAlertor(BUZZER_PIN);                              // Handle breathing and buzzer together
    } else {                                                                // If button is released 
        stopAlertorAndLED(BUZZER_PIN);                                      // Turn off buzzer and LED
    }
}

int main(void) {
    setup();                                                                // Call the setup function to initialize
    while (1) {                                                             // Infinite loop
        loop();                                                             // Call the loop function
    }
    return 0;                                                               // This line will never be reached
}

