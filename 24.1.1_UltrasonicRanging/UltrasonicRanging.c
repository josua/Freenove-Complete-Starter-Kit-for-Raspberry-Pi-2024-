/*
Filename    : UltrasonicRanging.c
Description : Get distance via UltrasonicRanging sensor
Author      : Philippe Jos
Modified    : 13/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/24.1.1_UltrasonicRanging
*/
#include <wiringPi.h>                                   // Include wiringPi library
#include <stdio.h>                                      // Include standard I/O library
#include <sys/time.h>                                   // Include system time library

#define TRIG_PIN 4                                      // Define the TRIG pin (GPIO 23 in WiringPi numbering)
#define ECHO_PIN 5                                      // Define the ECHO pin (GPIO 24 in WiringPi numbering)
#define MAX_DISTANCE 100                                // Define the maximum measured distance (in cm)
#define TIMEOUT MAX_DISTANCE * 60                       // Calculate timeout according to the maximum measured distance

#define ERROR_TIMEOUT -1                                // Define error code for timeout
#define OK 1                                            // Define OK code

// Helper function to get the current time in microseconds
long getCurrentTimeMicros() {
    struct timeval now;                                 // Create timeval struct to hold current time
    gettimeofday(&now, NULL);                           // Get current time
    return now.tv_sec * 1000000L + now.tv_usec;         // Return time in microseconds
}

// Helper function to wait for a pin to reach a specific state
int waitForPinState(int pin, int state, long timeout) {
    long startTime = getCurrentTimeMicros();            // Record the start time in microseconds
    while (digitalRead(pin) != state) {                 // Wait until the pin matches the desired state
        if (getCurrentTimeMicros() - startTime > timeout) { // If the wait time exceeds the timeout
            printf("Timeout waiting for pin state %d\n", state); // Print timeout error
            return ERROR_TIMEOUT;                       // Return timeout error
        }
    }
    return OK;                                          // Return OK if the desired state is achieved
}

// Function to measure pulse duration in microseconds
int pulseIn(int pin, int level, long timeout) {
    long startTime, endTime;                            // Variables to store start and end time

    // Wait for the pin to reach the desired level
    if (waitForPinState(pin, level, timeout) == ERROR_TIMEOUT) { // Wait for pin to reach level
        return 0;                                       // Return 0 if timeout
    }

    // Record the start time
    startTime = getCurrentTimeMicros();                 // Get current time as start time

    // Wait for the pin to go to the opposite level
    if (waitForPinState(pin, !level, timeout) == ERROR_TIMEOUT) { // Wait for pin to go low
        return 0;                                       // Return 0 if timeout
    }

    // Record the end time
    endTime = getCurrentTimeMicros();                   // Get current time as end time

    // Calculate the pulse duration
    return endTime - startTime;                         // Return the pulse duration
}

// Function to get the measurement result from the ultrasonic sensor (unit: cm)
float getSonar() {
    long pingTime;                                      // Variable to store pulse duration
    float distance;                                     // Variable to store calculated distance

    // Send 10us high level to TRIG_PIN
    digitalWrite(TRIG_PIN, HIGH);                       // Set TRIG_PIN high
    delayMicroseconds(10);                              // Wait for 10 microseconds
    digitalWrite(TRIG_PIN, LOW);                        // Set TRIG_PIN low

    // Read the pulse duration from ECHO_PIN
    pingTime = pulseIn(ECHO_PIN, HIGH, TIMEOUT);        // Measure the duration of the pulse

    // Calculate distance based on the pulse duration and sound speed (340 m/s)
    distance = (float)pingTime * 340.0 / 2.0 / 10000.0; // Calculate distance in cm

    return distance;                                    // Return the calculated distance
}

// Setup function to initialize the GPIO pins
void setup() {
    wiringPiSetup();                                    // Initialize wiringPi library
    pinMode(TRIG_PIN, OUTPUT);                          // Set TRIG_PIN as an output
    pinMode(ECHO_PIN, INPUT);                           // Set ECHO_PIN as an input
}

// Loop function to continuously measure and print the distance
void loop() {
    float distance = getSonar();                        // Get distance from the ultrasonic sensor
    printf("Distance: %.2f cm\n", distance);            // Print the distance
    delay(1000);                                        // Wait for 1 second before the next measurement
}

// Main function to call setup and loop functions
int main() {
    setup();                                            // Call the setup function
    while (1) {                                         // Infinite loop
        loop();                                         // Continuously call the loop function
    }
    return 0;                                           // This line will never be reached
}


