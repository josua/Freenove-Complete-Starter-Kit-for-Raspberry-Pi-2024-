/*
Filename    : LightWater02.c
Description : Control LED by 74HC595
Author      : Philippe Jos
Modified    : 17/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/17.1.1_LightWater02
*/
#include <stdio.h>                                              // Include standard I/O library
#include <wiringPi.h>                                           // Include wiringPi library
#include <wiringShift.h>                                        // Include wiringShift library

#define DATA_PIN 0                                              // DS Pin of 74HC595 (Pin 14)
#define LATCH_PIN 2                                             // ST_CP Pin of 74HC595 (Pin 12)
#define CLOCK_PIN 3                                             // CH_CP Pin of 74HC595 (Pin 11)

// Function to shift out data
void shiftOutData(int dataPin, int clockPin, int bitOrder, int value) {   
    for (int i = 0; i < 8; i++) {
        digitalWrite(clockPin, LOW);                            // Set clock pin LOW
        if (bitOrder == LSBFIRST) {                             // If order is LSBFIRST
            digitalWrite(dataPin, (value >> i) & 0x01);         // Write bit to data pin
        } else {                                                // If order is MSBFIRST
            digitalWrite(dataPin, (value << i) & 0x80);         // Write bit to data pin
        }
        delayMicroseconds(10);                                  // Small delay
        digitalWrite(clockPin, HIGH);                           // Set clock pin HIGH
        delayMicroseconds(10);                                  // Small delay
    }
}

// Function to initialize pins
void setup() {
    wiringPiSetup();                                            // Initialize wiringPi
    pinMode(DATA_PIN, OUTPUT);                                  // Set data pin as output
    pinMode(LATCH_PIN, OUTPUT);                                 // Set latch pin as output
    pinMode(CLOCK_PIN, OUTPUT);                                 // Set clock pin as output
}

// Function to shift a bit across the LEDs
void loop() {
    unsigned char value = 0x01;                                 // Initial value for shifting
    for (int i = 0; i < 8; i++) {                               // Loop to shift LED left
        digitalWrite(LATCH_PIN, LOW);                           // Set latch pin LOW
        shiftOutData(DATA_PIN, CLOCK_PIN, LSBFIRST, value);     // Shift out data
        digitalWrite(LATCH_PIN, HIGH);                          // Set latch pin HIGH to latch data
        value <<= 1;                                            // Shift bit left
        delay(100);                                             // Delay 100ms
    }
    value = 0x80;                                               // Initial value for shifting right
    for (int i = 0; i < 8; i++) {                               // Loop to shift LED right
        digitalWrite(LATCH_PIN, LOW);                           // Set latch pin LOW
        shiftOutData(DATA_PIN, CLOCK_PIN, LSBFIRST, value);     // Shift out data
        digitalWrite(LATCH_PIN, HIGH);                          // Set latch pin HIGH to latch data
        value >>= 1;                                            // Shift bit right
        delay(100);                                             // Delay 100ms
    }
}

// Main function
int main(void) {
    setup();                                                    // Initialize pins
    while (1) {                                                 // Infinite loop
        loop();                                                 // Shift LEDs
    }
    return 0;                                                   // Return 0 (never reached)
}

