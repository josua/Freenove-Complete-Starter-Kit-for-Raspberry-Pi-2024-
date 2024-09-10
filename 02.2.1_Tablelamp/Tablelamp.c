/*
Filename    : Tablelamp.c
Description : DIY MINI table lamp
Author      : Philippe Jos
Modified    : 06/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/00.0.0_Hello 
*/
#include <stdio.h>                              // Include standard I/O library
#include <wiringPi.h>                           // Include WiringPi library for GPIO control

#define LED_PIN       24                        // Define the LED pin
#define BUTTON_PIN    1                         // Define the button pin

int ledState = LOW;                             // Store the state of the LED (initially off)
int buttonState = HIGH;                         // Store the state of the button (initially not pressed)
int lastButtonState = HIGH;                     // Store the last state of the button
long lastChangeTime;                            // Store the change time of button state
const long captureTime = 50;                    // Set the debounce time for button state (50ms)
int reading;                                    // Variable to store the current reading of the button state

// Function to initialize the setup
void setup() {    
    printf("Program is starting...\n");         // Print starting message
    wiringPiSetup();                            // Initialize wiringPi
    pinMode(LED_PIN, OUTPUT);                   // Set LED_PIN to output
    pinMode(BUTTON_PIN, INPUT);                 // Set BUTTON_PIN to input
    pullUpDnControl(BUTTON_PIN, PUD_UP);        // Enable pull-up resistor on BUTTON_PIN
}

// Function to read the current state of the button
void readButtonState() {
    reading = digitalRead(BUTTON_PIN);          // Read the current state of the button
}

// Function to check if the button state has changed
void checkButtonStateChange() {
    if (reading != lastButtonState) {           // If the button state has changed
        lastChangeTime = millis();              // Record the time point of change
    }
}

// Function to handle actions based on the button state
void handleButtonAction() {
    if (buttonState == LOW) {                   // If the button is pressed
        printf("Button is pressed!\n");         // Print pressed message
        ledState = !ledState;                   // Toggle the LED state
        if (ledState) {                         // If LED is on
            printf("LED turned on\n");          // Print turn on message
        } else {                                // If LED is off
            printf("LED turned off\n");         // Print turn off message
        }
    } else {                                    // If the button is released
        printf("Button is released!\n");        // Print released message
    }
}

// Function to update the button state if the change is stable
void updateButtonState() {
    if (millis() - lastChangeTime > captureTime) { // Check if the change has lasted beyond the debounce time
        if (reading != buttonState) {              // If the stable state is different from the current state
            buttonState = reading;                 // Update the button state
            handleButtonAction();                  // Handle the button action based on the new state
        }
    }
}

// Function to update the state of the LED
void updateLEDState() {
    digitalWrite(LED_PIN, ledState);            // Write the LED state to LED_PIN
}

// The main loop function
void loop() {
    readButtonState();                          // Read the current button state
    checkButtonStateChange();                   // Check if the button state has changed
    updateButtonState();                        // Update the button state if the change is stable
    updateLEDState();                           // Update the LED state
    lastButtonState = reading;                  // Save the current reading as the last state
}

// The main function
int main(void) {
    setup();                                    // Call the setup function to initialize
    while (1) {                                 // Infinite loop
        loop();                                 // Call the loop function
    }
    return 0;                                   // This line will never be reached
}


