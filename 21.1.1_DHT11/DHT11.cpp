/*
Filename    : DHT11.cpp
Description : Read the temperature and humidity data of DHT11
Author      : freenove & Philippe Jos
Modified    : 16/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/21.1.1_DHT11
*/
#include <wiringPi.h>
#include <stdio.h>
#include "DHT.hpp"

// Define the pin where the DHT11 sensor is connected
#define DHT11_PIN 0

// Initialize the reading counter
int measurementCount = 0;

// Create a DHT object
DHT dhtSensor;

void loop() {
    measurementCount++;                                                   // Increment the reading counter
    printf("Measurement Count: %d\n", measurementCount);                  // Print the current measurement count

    int chk = dhtSensor.readDHT11(DHT11_PIN);                             // Read data from DHT11 sensor
    if (chk == DHTLIB_OK) {                                               // Check if the reading was successful
        printf("Sensor Status: OK\n");                                    // Print success message
    } else if (chk == DHTLIB_ERROR_CHECKSUM) {
        printf("Sensor Status: Checksum Error\n");                        // Print checksum error message
    } else if (chk == DHTLIB_ERROR_TIMEOUT) {
        printf("Sensor Status: Timeout Error\n");                         // Print timeout error message
    } else {
        printf("Sensor Status: Unknown Error\n");                         // Print unknown error message
    }

    // Print the humidity and temperature readings
    printf("Humidity: %.2f %%\t Temperature: %.2f °C\n", dhtSensor.humidity, dhtSensor.temperature);
    printf("-------------------------------------------------\n");
    delay(2000);                                                          // Wait for 2 seconds before the next reading
}

int main(void) {
    while (true) {                                                        // Infinite loop to continuously read from the sensor
        loop();                                                           // Read from the DHT11 sensor
    }
    return 0;                                                             // Return 0 (never reached)
}



