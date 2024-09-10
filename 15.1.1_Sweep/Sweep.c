/*
Filename    : Sweep.c
Description : Servo 180° Rotation
Author      : Philippe Jos
Modified    : 10/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/15.1.1_Sweep
*/

/**
 * Servo Control Code
 * Time (ms)  |  Angle (°)
 * ------------------------
 *    0.5     |     0
 *    1.0     |    45
 *    1.5     |    90
 *    2.0     |   135
 *    2.5     |   180
 **/

#include <stdio.h>                                             // Include the standard I/O library
#include <wiringPi.h>                                          // Include the wiringPi library
#include <softPwm.h>                                           // Include the soft PWM library

#define OFFSET_MS 1                                            // Offset for servo pulse in 0.1ms units
#define SERVO_MIN_PULSE_WIDTH 5 + OFFSET_MS                    // Minimum pulse width for servo in 0.1ms units
#define SERVO_MAX_PULSE_WIDTH 25 + OFFSET_MS                   // Maximum pulse width for servo in 0.1ms units
#define SERVO_PIN 1                                            // GPIO 18 pin number connected to the servo

// Function to map a value from one range to another
long mapValue(long value, long fromLow, long fromHigh, long toLow, long toHigh) {
    return (toHigh - toLow) * (value - fromLow) / (fromHigh - fromLow) + toLow;
}

// Function to set the servo angle (0-180 degrees)
void setServoAngle(int pin, int angle) {
    if (angle > 180) angle = 180;                              // Ensure angle does not exceed 180
    if (angle < 0) angle = 0;                                  // Ensure angle does not go below 0
    int pulseWidth = mapValue(angle, 0, 180, SERVO_MIN_PULSE_WIDTH, SERVO_MAX_PULSE_WIDTH); // Map angle to pulse width
    softPwmWrite(pin, pulseWidth);                             // Set the PWM value to the servo pin
}

// Function to set the servo pulse width in 0.1ms units
void setServoPulseWidth(int pin, int pulseWidth) {
    if (pulseWidth > SERVO_MAX_PULSE_WIDTH) pulseWidth = SERVO_MAX_PULSE_WIDTH; // Ensure pulse width does not exceed max
    if (pulseWidth < SERVO_MIN_PULSE_WIDTH) pulseWidth = SERVO_MIN_PULSE_WIDTH; // Ensure pulse width does not go below min
    softPwmWrite(pin, pulseWidth);                             // Set the PWM value to the servo pin
}

// Setup function to initialize the servo pin
void setup() {
    wiringPiSetup();                                           // Initialize wiringPi
    softPwmCreate(SERVO_PIN, 0, 200);                          // Initialize PWM on the servo pin with a range of 200
}

// Loop function to rotate the servo back and forth
void loop() {
    // Rotate servo from minimum angle to maximum angle
    for (int i = 0; i <= 180; i++) {                           // Loop from 0 to 180 degrees
        setServoAngle(SERVO_PIN, i);                           // Set servo angle
        delay(5);                                              // Delay for 5ms
    }
    delay(500);                                                // Delay for 500ms

    // Rotate servo from maximum angle to minimum angle
    for (int i = 180; i >= 0; i--) {                           // Loop from 180 to 0 degrees
        setServoAngle(SERVO_PIN, i);                           // Set servo angle
        delay(5);                                              // Delay for 5ms
    }
    delay(500);                                                // Delay for 500ms
}

// Main function to start the setup and loop functions
int main(void) {
    setup();                                                   // Initialize pins and PWM
    while (1) {                                                // Infinite loop
        loop();                                                // Continuously rotate the servo
    }
    return 0;                                                  // Return 0 (never reached)
}
