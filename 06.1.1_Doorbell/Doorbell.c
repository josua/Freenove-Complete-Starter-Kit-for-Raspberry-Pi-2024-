/*
Filename    : Doorbell.c
Description : Make doorbell with buzzer and button.
Author      : Philippe Jos
Modified    : 06/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/06.1.1_Doorbell
*/
#include <stdio.h>                                                         // Include standard I/O library
#include <wiringPi.h>                                                      // Include WiringPi library

#define BUZZER_PIN 0                                                       // Define the buzzer pin
#define BUTTON_PIN 1                                                       // Define the button pin

// Setup function to initialize wiringPi and configure pins
void setup() {
    wiringPiSetup();                                                       // Initialize WiringPi
    pinMode(BUZZER_PIN, OUTPUT);                                           // Set buzzer pin as output
    pinMode(BUTTON_PIN, INPUT);                                            // Set button pin as input
    pullUpDnControl(BUTTON_PIN, PUD_UP);                                   // Enable pull-up resistor on button pin
}

// Loop function to check button state and control buzzer
void loop() {
    if (digitalRead(BUTTON_PIN) == LOW) {                                  // If button is pressed
        digitalWrite(BUZZER_PIN, HIGH);                                    // Turn on buzzer
        printf("Buzzer turned on\n");                                      // Print buzzer on message
    } else {                                                               // If button is released
        digitalWrite(BUZZER_PIN, LOW);                                     // Turn off buzzer
        printf("Buzzer turned off\n");                                     // Print buzzer off message
    }
    delay(100);                                                            // Small delay to debounce button
}

// Main function to set up and repeatedly call the loop function
int main(void) {
    setup();                                                               // Call the setup function to initialize
    while (1) {                                                            // Infinite loop
        loop();                                                            // Call the loop function
    }
    return 0;                                                              // Return 0 (never reached)
}


