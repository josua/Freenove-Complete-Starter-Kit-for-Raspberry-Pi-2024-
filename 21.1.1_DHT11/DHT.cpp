/**********************************************************************
* Filename    : DHT.cpp
* Description : DHT Temperature & Humidity Sensor library for Raspberry.
*               Used for Raspberry Pi.
*               Program transplantation by Freenove.
* Author      : freenove & Philippe Jos
* Modified    : 16/07/2024
* Reference   : https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib
**********************************************************************/
#include "DHT.hpp"

DHT::DHT() {                                                                                                   
    wiringPiSetup();                                      // Initialize wiringPi
}

// Function to read sensor and store data in bits[]
// Returns: DHTLIB_OK, DHTLIB_ERROR_CHECKSUM, DHTLIB_ERROR_TIMEOUT
int DHT::readSensor(int pin, int wakeupDelay) {
    int mask = 0x80;                                     // Bit mask for reading data
    int idx = 0;                                         // Index for data array
    int i;                                               // Loop counter
    int32_t t;                                           // Timing variable

    for (i = 0; i < 5; i++) {                            // Initialize data array
        bits[i] = 0;                                     // Set each bit to 0
    }

    pinMode(pin, OUTPUT);                                // Set pin mode to output
    digitalWrite(pin, HIGH);                             // Set pin to high
    delay(500);                                          // Wait for 500 ms

    digitalWrite(pin, LOW);                              // Start signal: set pin to low
    delay(wakeupDelay);                                  // Wait for wakeup delay
    digitalWrite(pin, HIGH);                             // Set pin to high

    pinMode(pin, INPUT);                                 // Set pin mode to input
    
    int32_t loopCnt = DHTLIB_TIMEOUT;                    // Timeout counter
    t = micros();                                        // Get current time
    while (digitalRead(pin) == HIGH) {                   // Wait for sensor response
        if ((micros() - t) > loopCnt) {                  // Check for timeout
            return DHTLIB_ERROR_TIMEOUT;                 // Return timeout error
        }
    }
    
    loopCnt = DHTLIB_TIMEOUT;                            // Reset timeout counter
    t = micros();                                        // Get current time
    while (digitalRead(pin) == LOW) {                    // Wait for sensor to pull low
        if ((micros() - t) > loopCnt) {                  // Check for timeout
            return DHTLIB_ERROR_TIMEOUT;                 // Return timeout error
        }
    }

    loopCnt = DHTLIB_TIMEOUT;                            // Reset timeout counter
    t = micros();                                        // Get current time
    while (digitalRead(pin) == HIGH) {                   // Wait for sensor to pull high
        if ((micros() - t) > loopCnt) {                  // Check for timeout
            return DHTLIB_ERROR_TIMEOUT;                 // Return timeout error
        }
    }

    for (i = 0; i < 40; i++) {                           // Read 40 bits from sensor
        loopCnt = DHTLIB_TIMEOUT;                        // Reset timeout counter
        t = micros();                                    // Get current time
        while (digitalRead(pin) == LOW) {                // Wait for pin to go low
            if ((micros() - t) > loopCnt) {              // Check for timeout
                return DHTLIB_ERROR_TIMEOUT;             // Return timeout error
            }
        }
        t = micros();                                    // Get current time
        loopCnt = DHTLIB_TIMEOUT;                        // Reset timeout counter
        while (digitalRead(pin) == HIGH) {               // Wait for pin to go high
            if ((micros() - t) > loopCnt) {              // Check for timeout
                return DHTLIB_ERROR_TIMEOUT;             // Return timeout error
            }
        }
        if ((micros() - t) > 60) {                       // Check if high signal lasted long enough
            bits[idx] |= mask;                           // Set bit in data array
        }
        mask >>= 1;                                      // Shift mask right
        if (mask == 0) {                                 // If mask is 0
            mask = 0x80;                                 // Reset mask to 0x80
            idx++;                                       // Move to next byte in data array
        }
    }

    pinMode(pin, OUTPUT);                                // Set pin mode to output
    digitalWrite(pin, HIGH);                             // Set pin to high
    return DHTLIB_OK;                                    // Return OK
}

// Function to read DHT11 sensor once and analyze the data
// Returns: DHTLIB_OK, DHTLIB_ERROR_CHECKSUM, DHTLIB_ERROR_TIMEOUT
int DHT::readDHT11Once(int pin) {
    int rv;                                              // Return value
    uint8_t sum;                                         // Checksum

    rv = readSensor(pin, DHTLIB_DHT11_WAKEUP);           // Read sensor data
    if (rv != DHTLIB_OK) {                               // Check if reading was successful
        humidity = DHTLIB_INVALID_VALUE;                 // Set invalid humidity value
        temperature = DHTLIB_INVALID_VALUE;              // Set invalid temperature value
        return rv;                                       // Return error code
    }

    humidity = bits[0];                                  // Extract humidity
    temperature = bits[2] + bits[3] * 0.1;               // Extract temperature
    sum = bits[0] + bits[1] + bits[2] + bits[3];         // Calculate checksum
    if (bits[4] != sum) {                                // Check if checksum is valid
        return DHTLIB_ERROR_CHECKSUM;                    // Return checksum error
    }

    return DHTLIB_OK;                                    // Return OK
}

// Function to read DHT11 sensor with retries
// Returns: DHTLIB_OK, DHTLIB_ERROR_CHECKSUM, DHTLIB_ERROR_TIMEOUT
int DHT::readDHT11(int pin) {
    int chk = DHTLIB_INVALID_VALUE;                      // Check value
    for (int i = 0; i < 15; i++) {                       // Try up to 15 times
        chk = readDHT11Once(pin);                        // Read sensor once
        if (chk == DHTLIB_OK) {                          // If read was successful
            return DHTLIB_OK;                            // Return OK
        }
        delay(100);                                      // Wait 100 ms before retrying
    }
    return chk;                                          // Return last check value
}

