/*
Filename    : MPU6050RAW.c
Description : Read data of MPU6050
Author      : Philippe Jos
Modified    : 13/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/25.1.1_MPU6050
*/
#include <stdio.h>                                      // Include standard I/O library
#include <stdint.h>                                     // Include standard integer types
#include <unistd.h>                                     // Include Unix standard library for sleep functions
#include "I2Cdev.h"                                     // Include I2Cdev library for I2C communication
#include "MPU6050.h"                                    // Include MPU6050 library for sensor communication

MPU6050 accelgyro;                                      // Create MPU6050 class object

int16_t ax, ay, az;                                     // Variables to store acceleration data
int16_t gx, gy, gz;                                     // Variables to store gyroscope data

// Setup function to initialize the MPU6050 sensor
void setup() {
    // Initialize device
    printf("Initializing I2C devices...\n");            // Print initializing message
    accelgyro.initialize();                             // Initialize MPU6050

    // Verify connection
    printf("Testing device connections...\n");          // Print connection test message
    if (accelgyro.testConnection()) {                   // Test connection to MPU6050
        printf("MPU6050 connection successful\n");      // Print success message
    } else {
        printf("MPU6050 connection failed\n");          // Print failure message
    }
}

// Loop function to continuously read and display sensor data
void loop() {
    // Read accel/gyro values from MPU6050
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // Get motion values

    // Display headers
    printf("Raw Accel/Gyro:     AX     AY     AZ     GX     GY     GZ\n"); // Print headers

    // Display raw accel/gyro x/y/z values
    printf("Raw values:      %6hd %6hd %6hd   %6hd %6hd %6hd\n", ax, ay, az, gx, gy, gz); // Print raw values

    // Convert and display accel values in g and gyro values in d/s with better alignment
    printf("Accel (g):       X: %7.2f   Y: %7.2f   Z: %7.2f\n",
           (float)ax / 16384.0, (float)ay / 16384.0, (float)az / 16384.0); // Print accel values

    printf("Gyro  (d/s):     X: %7.2f   Y: %7.2f   Z: %7.2f\n",
           (float)gx / 131.0, (float)gy / 131.0, (float)gz / 131.0); // Print gyro values
    
    usleep(500000);                                     // Delay for 500 milliseconds
}

// Main function to call setup and loop functions
int main() {
    setup();                                            // Call the setup function to initialize the sensor
    while (1) {                                         // Infinite loop
        loop();                                         // Continuously call the loop function
    }
    return 0;                                           // This line will never be reached
}

