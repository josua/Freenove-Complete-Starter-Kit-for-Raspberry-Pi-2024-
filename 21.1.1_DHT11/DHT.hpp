/**********************************************************************
* Filename    : DHT.hpp
* Description : DHT Temperature & Humidity Sensor library for Raspberry.
                Used for Raspberry Pi.
*               Program transplantation by Freenove.
* Author      : freenove & Philippe Jos
* Modified    : 16/07/2024
* Reference   : https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib
**********************************************************************/
#ifndef _DHT_H_                              // Include guard to prevent multiple inclusions
#define _DHT_H_

#include <wiringPi.h>                        // Include wiringPi library
#include <stdio.h>                           // Include standard I/O library
#include <stdint.h>                          // Include standard integer types

// Return flags for sensor reading
#define DHTLIB_OK               0            // Success flag
#define DHTLIB_ERROR_CHECKSUM   -1           // Checksum error flag
#define DHTLIB_ERROR_TIMEOUT    -2           // Timeout error flag
#define DHTLIB_INVALID_VALUE    -999         // Invalid value flag

#define DHTLIB_DHT11_WAKEUP     20           // Wake up delay for DHT11
#define DHTLIB_DHT_WAKEUP       1            // Wake up delay for DHT
#define DHTLIB_TIMEOUT          100          // Timeout for reading

class DHT {      
    public:
        DHT();                               // Constructor
        double humidity, temperature;        // Variables to store temperature and humidity data
        int readDHT11Once(int pin);          // Read DHT11 sensor once
        int readDHT11(int pin);              // Read DHT11 sensor with retries
    private:
        uint8_t bits[5];                     // Buffer to store sensor data
        int readSensor(int pin, int wakeupDelay);  // Read sensor with specified wakeup delay
};

#endif                                        // End of include guard




