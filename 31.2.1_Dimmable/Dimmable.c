/*
Filename    : Dimmable.c
Description : Use rotary encoder to control LED brightness.  
Author      : Philippe Jos
Modified    : 15/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/31.2.1_Dimmable
*/
#include <stdio.h>                                              // Include standard I/O library
#include <wiringPi.h>                                           // Include WiringPi library for GPIO control
#include <softPwm.h>                                            // Include SoftPWM library for PWM control

#define CLK_PIN   0                                             // Define the CLK pin for the rotary encoder
#define DT_PIN    1                                             // Define the DT pin for the rotary encoder
#define SW_PIN    2                                             // Define the switch pin for the rotary encoder
#define LED_PIN   3                                             // Define the LED pin for PWM output

int previousCounterValue = 0;                                   // Store the previous counter value
int currentCounterValue  = 0;                                   // Store the current counter value

// Function to read and process the rotary encoder input
void readRotaryEncoder() {
    static int lastCLKStatus = digitalRead(CLK_PIN);            // Initialize last state of CLK pin to its actual state
    int currentCLKStatus = digitalRead(CLK_PIN);                // Read the current state of the CLK pin
    
    if (currentCLKStatus != lastCLKStatus &&                    // Check if the state of the CLK pin has changed
        currentCLKStatus == LOW) {                              // and the current state is LOW
        if (digitalRead(DT_PIN) != currentCLKStatus) {          // Check the state of the DT pin
            previousCounterValue++;                             // Increment the counter if DT is different from CLK
        } else {
            previousCounterValue--;                             // Decrement the counter if DT is the same as CLK
        }
    }
    lastCLKStatus = currentCLKStatus;                           // Update the last state of the CLK pin
}

void setup() {
    wiringPiSetup();                                            // Initialize WiringPi library
    pinMode(CLK_PIN, INPUT);                                    // Set CLK pin as input
    pinMode(DT_PIN, INPUT);                                     // Set DT pin as input
    pinMode(SW_PIN, INPUT);                                     // Set SW pin as input
    pullUpDnControl(SW_PIN, PUD_UP);                            // Enable pull-up resistor on SW pin
    softPwmCreate(LED_PIN, 0, 20);                              // Create a software PWM on the LED pin with range 0-20
    printf("Counter = %d\n", previousCounterValue);             // Print initial counter value
}

void loop() {
    readRotaryEncoder();                                        // Read and process rotary encoder input
    
    if (digitalRead(SW_PIN) == LOW) {                           // Check if the switch is pressed
        delay(100);                                             // Debounce delay
        if (digitalRead(SW_PIN) == LOW) {                       // Check again if the switch is pressed
            previousCounterValue = 0;                           // Reset the counter value
        }
    }

    if (previousCounterValue >= 20) {                           // Limit the counter value to a maximum of 20
        previousCounterValue = 20;
    }
    if (previousCounterValue <= 0) {                            // Limit the counter value to a minimum of 0
        previousCounterValue = 0;
    }

    if (currentCounterValue != previousCounterValue) {          // Check if the counter value has changed
        printf("Counter = %d\n", previousCounterValue);         // Print the current counter value
        currentCounterValue = previousCounterValue;             // Update the current counter value
    }

    softPwmWrite(LED_PIN, previousCounterValue);                // Set the PWM output to the counter value
}

int main(void) {
    setup();                                                    // Call the setup function
    while (1) {                                                 // Infinite loop
        loop();                                                 // Continuously call the loop function
    }
    return 0;                                                   // Return 0 (this line will never be reached)
}

