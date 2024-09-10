/*
Filename    : ColorfulLED.c
Description : Random color change ColorfulLED
Author      : Philippe Jos
Modified    : 06/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/05.1.1_ColorfulLED
*/
#include <stdio.h>                                                         // Include standard I/O library
#include <stdlib.h>                                                        // Include standard library
#include <time.h>                                                          // Include time library for seeding random numbers
#include <wiringPi.h>                                                      // Include WiringPi library
#include <softPwm.h>                                                       // Include soft PWM library

#define LED_PIN_RED    0                                                   // Define the red LED pin
#define LED_PIN_GREEN  1                                                   // Define the green LED pin
#define LED_PIN_BLUE   2                                                   // Define the blue LED pin
#define PWM_RANGE      100                                                 // Define the PWM range
#define DELAY_TIME     1000                                                // Define delay time in milliseconds

// Function to set up LED pins for PWM
void setupLedPins(void) {
    softPwmCreate(LED_PIN_RED, 0, PWM_RANGE);                              // Create soft PWM for red LED pin
    softPwmCreate(LED_PIN_GREEN, 0, PWM_RANGE);                            // Create soft PWM for green LED pin
    softPwmCreate(LED_PIN_BLUE, 0, PWM_RANGE);                             // Create soft PWM for blue LED pin
}

// Function to set the color of the RGB LED
void setLedColor(int r, int g, int b) {
    softPwmWrite(LED_PIN_RED, r);                                          // Set PWM value for red LED
    softPwmWrite(LED_PIN_GREEN, g);                                        // Set PWM value for green LED
    softPwmWrite(LED_PIN_BLUE, b);                                         // Set PWM value for blue LED
}

// Function to generate a random PWM value
int getRandomPWMValue(void) {
    return rand() % PWM_RANGE;                                             // Generate random value within PWM range
}

// Function to update and display the RGB values
void updateAndDisplayColor(void) {
    int r = getRandomPWMValue();                                           // Generate random value for red LED
    int g = getRandomPWMValue();                                           // Generate random value for green LED
    int b = getRandomPWMValue();                                           // Generate random value for blue LED
    setLedColor(r, g, b);                                                  // Set the LED color
    printf("r=%d, g=%d, b=%d\n", r, g, b);                                 // Print the RGB values
}

// Setup function
void setup() {
    wiringPiSetup();                                                       // Initialize WiringPi
    setupLedPins();                                                        // Set up LED pins for PWM
    srand(time(NULL));                                                     // Seed the random number generator
}

// Loop function
void loop() {
    updateAndDisplayColor();                                               // Update and display the LED color
    delay(DELAY_TIME);                                                     // Delay for specified time
}

int main(void) {
    setup();                                                               // Call the setup function to initialize
    while (1) {                                                            // Infinite loop
        loop();                                                            // Call the loop function
    }
    return 0;                                                              // Return 0 (never reached)
}
