/*
Filename    : Relay.c
Description : Control Motor with Button and Relay
Author      : Philippe Jos
Modified    : 10/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/14.1.1_Relay
*/
#include <stdio.h>                              // Include standard I/O library
#include <wiringPi.h>                           // Include WiringPi library for GPIO

// Define pin numbers
#define RELAY_PIN 0                             // Relay pin (WiringPi pin 0, GPIO 17)
#define BUTTON_PIN 1                            // Button pin (WiringPi pin 1, GPIO 18)

// Initialize states and timing variables
int relayState = LOW;                           // Relay state, LOW means OFF
int buttonState = HIGH;                         // Button state, HIGH means not pressed
int lastButtonState = HIGH;                     // Last known button state
unsigned long lastChangeTime = 0;               // Last time the button state changed
const unsigned long debounceTime = 50;          // Debounce time to filter out button noise

// Setup function to initialize the pins
void setup() {
    wiringPiSetup();                            // Initialize WiringPi
    pinMode(RELAY_PIN, OUTPUT);                 // Set relay pin as output
    pinMode(BUTTON_PIN, INPUT);                 // Set button pin as input
    pullUpDnControl(BUTTON_PIN, PUD_UP);        // Enable pull-up resistor on button pin
}

// Main loop function to handle button press and relay control
void loop() {
    int currentState = digitalRead(BUTTON_PIN); // Read the current state of the button

    // If the button state has changed, reset the debounce timer
    if (currentState != lastButtonState) {
        lastChangeTime = millis();
    }

    // If the change in state lasts longer than the debounce time, consider it valid
    if ((millis() - lastChangeTime) > debounceTime) {
        // If the button state has changed, update the button state
        if (currentState != buttonState) {
            buttonState = currentState;

            // If the button is pressed (LOW), toggle the relay state
            if (buttonState == LOW) {
                relayState = !relayState;       // Toggle relay state

                // Print the new relay state
                if (relayState) {
                    printf("Relay on\n");
                } else {
                    printf("Relay off\n");
                }
            }
        }
    }

    // Set the relay pin to the relay state
    digitalWrite(RELAY_PIN, relayState);

    // Update the last button state
    lastButtonState = currentState;

    // Small delay to prevent excessive CPU usage
    delay(10);
}

// Main function to start the setup and loop functions
int main(void) {
    setup();                // Call setup function to initialize pins
    while (1) {             // Infinite loop
        loop();             // Call loop function to handle button and relay
    }
    return 0;               // Return 0 (never reached)
}
