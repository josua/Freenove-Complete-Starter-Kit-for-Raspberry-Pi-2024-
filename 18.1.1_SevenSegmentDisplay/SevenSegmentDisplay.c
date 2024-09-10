/*
Filename    : SevenSegmentDisplay.c
Description : SevenSegmentDisplay controlled by 74HC595
Author      : Philippe Jos
Modified    : 12/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/18.1.1_SevenSegmentDisplay
*/
#include <stdio.h>                                  // Include standard I/O library
#include <wiringPi.h>                               // Include wiringPi library for GPIO
#include <wiringShift.h>                            // Include wiringShift library for shift register functions

// Define pins connected to the 74HC595 shift register
#define DATA_PIN 0                                   // DS Pin of 74HC595 (Pin 14)
#define LATCH_PIN 2                                  // ST_CP Pin of 74HC595 (Pin 12)
#define CLOCK_PIN 3                                  // CH_CP Pin of 74HC595 (Pin 11)

// Array representing the segments for digits 0-9 and A-F on a 7-segment display
unsigned char digitSegments[] = {                   // Array of segment values
    0xc0, 0xf9, 0xa4, 0xb0,                         // Segments for digits 0, 1, 2, 3
    0x99, 0x92, 0x82, 0xf8,                         // Segments for digits 4, 5, 6, 7
    0x80, 0x90, 0x88, 0x83,                         // Segments for digits 8, 9, A, b
    0xc6, 0xa1, 0x86, 0x8e                          // Segments for digits C, d, E, F
};

// Function to shift out a byte of data to the shift register
void shiftOut(int dataPin, int clockPin, int bitOrder, int value) {
    for (int i = 0; i < 8; i++) {                   // Loop through each bit
        digitalWrite(clockPin, LOW);                // Set clock pin LOW
        if (bitOrder == MSBFIRST) {                 // If MSB first
            digitalWrite(dataPin, ((0x80 & (value << i)) == 0x80) ? HIGH : LOW); // Write bit to data pin
        } else if (bitOrder == LSBFIRST) {          // If LSB first
            digitalWrite(dataPin, ((0x01 & (value >> i)) == 0x01) ? HIGH : LOW); // Write bit to data pin
        }
        digitalWrite(clockPin, HIGH);               // Set clock pin HIGH
        delayMicroseconds(10);                      // Small delay
    }
}

// Setup function to initialize the GPIO pins
void setup() {
    wiringPiSetup();                                // Initialize wiringPi library
    pinMode(DATA_PIN, OUTPUT);                      // Set data pin as output
    pinMode(LATCH_PIN, OUTPUT);                     // Set latch pin as output
    pinMode(CLOCK_PIN, OUTPUT);                     // Set clock pin as output
}

// Loop function to cycle through the digit segments
void loop() {
    for (int i = 0; i < sizeof(digitSegments); i++) {// Loop through each segment
        digitalWrite(LATCH_PIN, LOW);                // Set latch pin LOW
		// Shift out data to shift register
        shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitSegments[i] & 0x7f); // Display the digit segment and the dot
        digitalWrite(LATCH_PIN, HIGH);              // Set latch pin HIGH
        delay(500);                                 // Delay to observe the display
    }
}

// Main function to start the setup and loop functions
int main(void) {
    setup();                                        // Initialize pins and setup the system
    while (true) {                                  // Infinite loop
        loop();                                     // Continuously cycle through the digits
    }
    return 0;                                       // Return 0 (never reached)
}


