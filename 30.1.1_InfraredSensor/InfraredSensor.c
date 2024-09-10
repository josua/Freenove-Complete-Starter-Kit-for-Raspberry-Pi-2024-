/*
Filename    : InfraredSensor.c
Description : Infrared Obstacle Avoidance Sensor control LED
Author      : Philippe Jos
Modified    : 15/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/30.1.1_InfraredSensor
*/
#include <stdio.h>                                          // Include standard I/O library
#include <cstdlib>                                          // Include standard library for exit() function
#include <string.h>                                         // Include string library for string manipulation
#include <wiringPi.h>                                       // Include wiringPi library for GPIO control

#define LED_PIN    0                                        // Define the LED pin
#define SENSOR_PIN 1                                        // Define the sensor pin

char waitingAnimation[19];                                  // String to store the waiting animation (18 dots max)
int animationDirection = 1;                                 // 1 for increasing, -1 for decreasing
bool firstAnimation = true;                                 // Flag to check if this is the first animation cycle
bool obstacleDetected = false;                              // Flag to track if obstacle is detected

// Function to update the waiting animation
void updateWaitingAnimation() {
    int len = strlen(waitingAnimation);                     // Get the current length of the animation string
    if (len >= 18) {                                        // If the string length reaches the maximum
        animationDirection = -1;                            // Change direction to decreasing
    } else if (len <= 0) {                                  // If the string length is zero
        animationDirection = 1;                             // Change direction to increasing
    }

    if (animationDirection == 1) {                          // If increasing direction
        strcat(waitingAnimation, ".");                      // Append a dot to the string
    } else {                                                // If decreasing direction
        waitingAnimation[len - 1] = '\0';                   // Remove a dot from the string
    }
}

// Setup function to initialize wiringPi and configure pins
void setup() {
    printf("Program is starting...\n");
    if (wiringPiSetup() == -1) {                            // Initialize wiringPi and check for setup errors
        printf("Setup wiringPi failed!\n");
        exit(1);                                            // Exit the program if setup fails
    }
    pinMode(LED_PIN, OUTPUT);                               // Set LED pin to output mode
    pinMode(SENSOR_PIN, INPUT);                             // Set sensor pin to input mode
    waitingAnimation[0] = '\0';                             // Initialize the waiting animation string
    printf("Setup complete. Sensor pin: %d, LED pin: %d\n", SENSOR_PIN, LED_PIN);
}

// Loop function to check sensor and update animation
void loop() {
    if (digitalRead(SENSOR_PIN) == LOW) {                   // If obstacle detected
        digitalWrite(LED_PIN, HIGH);                        // Turn on LED
        if (!obstacleDetected) {                            // If the obstacle was not detected in the previous iteration
            printf("\nObstacle detected!");                 // Print the detection message with spaces to clear the line
            fflush(stdout);                                 // Ensure the output is printed immediately
            obstacleDetected = true;                        // Set the obstacle detected flag to true
        }
        waitingAnimation[0] = '\0';                         // Reset the waiting animation string
        firstAnimation = true;                              // Reset the animation flag
    } else {                                                // If no obstacle detected
        digitalWrite(LED_PIN, LOW);                         // Turn off LED
        if (firstAnimation) {                               // If it's the first animation cycle
            printf("\n");                                   // Print a newline before starting the animation
            firstAnimation = false;                         // Reset the flag
        }
        updateWaitingAnimation();                           // Update the waiting animation
        printf("\r%-18s", waitingAnimation);                // Print the waiting animation string with padding to clear previous characters
        printf("\r%s", waitingAnimation);                   // Ensure the cursor returns to the start of the line and prints the updated animation
        fflush(stdout);                                     // Ensure the output is printed immediately
        obstacleDetected = false;                           // Reset the obstacle detected flag
    }
    delay(25);                                              // Delay to make the output more readable and reduce CPU usage
}

// Main function to start the setup and loop functions
int main(void) {
    setup();                                                // Call the setup function
    while (1) {                                             // Infinite loop
        loop();                                             // Continuously call the loop function
    }
    return 0;                                               // Return 0 (this line will never be reached)
}
