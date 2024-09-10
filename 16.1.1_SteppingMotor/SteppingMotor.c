/*
Filename    : SteppingMotor.c
Description : Stepping Motor Driver
Author      : Philippe Jos
Modified    : 11/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/16.1.1_SteppingMotor

Note: The stator in the Stepper Motor supplied has 32 magnetic poles. 
Therefore, to complete one full revolution requires 32 full steps.
The rotor (or output shaft) of the Stepper Motor is connected to a speed
reduction set of gears and the reduction ratio is 1:64.
Therefore, the final output shaft (exiting the Stepper Motor's housing)
requires 32 X 64 = 2048 steps to make one full revolution. 
*/
#include <stdio.h>                                              // Include standard I/O library
#include <wiringPi.h>                                           // Include wiringPi library

// Define pins connected to the four phases (ABCD) of the stepper motor
const int motorPins[] = {1, 4, 5, 6};    

// Define power supply order for coil for rotating anticlockwise
const int anticlockwiseSteps[] = {0x01, 0x02, 0x04, 0x08};

// Define power supply order for coil for rotating clockwise
const int clockwiseSteps[] = {0x08, 0x04, 0x02, 0x01};

// Function to drive the stepper motor one period (4 steps) in a specified direction
void moveOnePeriod(int direction, int delayMs) {
    for (int step = 0; step < 4; step++) {                      // Cycle through the power supply order
        for (int pin = 0; pin < 4; pin++) {                     // Assign each pin, a total of 4 pins
            if (direction == 1) {                               // Power supply order for clockwise rotation
                digitalWrite(motorPins[pin], (anticlockwiseSteps[step] == (1 << pin)) ? HIGH : LOW);
            } else {                                            // Power supply order for anticlockwise rotation
                digitalWrite(motorPins[pin], (clockwiseSteps[step] == (1 << pin)) ? HIGH : LOW);
            }
        }
        delay(delayMs < 3 ? 3 : delayMs);                       // Delay, ensuring it is not less than 3ms
    }
}

// Function to rotate the motor continuously for a specified number of steps
// 'steps' specifies the number of cycles, where each cycle consists of 4 steps
void moveSteps(int direction, int delayMs, int cycles) {
    for (int i = 0; i < cycles; i++) {
        moveOnePeriod(direction, delayMs);                      // Move one period in the specified direction
    }
}

// Function to stop the motor
void stopMotor() {
    for (int pin = 0; pin < 4; pin++) {
        digitalWrite(motorPins[pin], LOW);                      // Set all motor pins to LOW
    }
}

// Setup function to initialize the motor pins
void setup() {
    wiringPiSetup();                                            // Initialize wiringPi
    for (int pin = 0; pin < 4; pin++) {
        pinMode(motorPins[pin], OUTPUT);                        // Set motor pins as output
    }
}

// Loop function to rotate the motor back and forth
void loop() {
    moveSteps(1, 3, 512);                                       // Rotate 360° clockwise (2048 steps in a full circle, i.e., 512 cycles)
    delay(500);                                                 // Delay for 500ms

    moveSteps(0, 3, 512);                                       // Rotate 360° anticlockwise
    delay(500);                                                 // Delay for 500ms
}

// Main function to start the setup and loop functions
int main(void) {
    setup();                                                    // Initialize pins and PWM
    while (true) {
        loop();                                                 // Continuously rotate the motor
    }
    return 0;                                                   // Return 0 (never reached)
}

