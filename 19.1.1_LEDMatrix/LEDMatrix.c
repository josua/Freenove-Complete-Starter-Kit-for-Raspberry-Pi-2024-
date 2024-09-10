/*
Filename    : LEDMatrix.c
Description : Control LEDMatrix by 74HC595
Author      : Philippe Jos
Modified    : 12/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/19.1.1_LEDMatrix
*/
#include <stdio.h>                             // Include standard I/O library
#include <wiringPi.h>                          // Include wiringPi library for GPIO
#include <wiringShift.h>                       // Include wiringShift library for shift register functions

#define DATA_PIN 0                             // DS Pin of 74HC595 (Pin 14)
#define LATCH_PIN 2                            // ST_CP Pin of 74HC595 (Pin 12)
#define CLOCK_PIN 3                            // SH_CP Pin of 74HC595 (Pin 11)

// Data for a smiley face
unsigned char smileyFace[] = {
    0x1c, 0x22, 0x51, 0x45, 0x45, 0x51, 0x22, 0x1c
};

// Data for characters "0" to "F"
unsigned char hexChars[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // " "
    0x00, 0x00, 0x3E, 0x41, 0x41, 0x3E, 0x00, 0x00, // "0"
    0x00, 0x00, 0x21, 0x7F, 0x01, 0x00, 0x00, 0x00, // "1"
    0x00, 0x00, 0x23, 0x45, 0x49, 0x31, 0x00, 0x00, // "2"
    0x00, 0x00, 0x22, 0x49, 0x49, 0x36, 0x00, 0x00, // "3"
    0x00, 0x00, 0x0E, 0x32, 0x7F, 0x02, 0x00, 0x00, // "4"
    0x00, 0x00, 0x79, 0x49, 0x49, 0x46, 0x00, 0x00, // "5"
    0x00, 0x00, 0x3E, 0x49, 0x49, 0x26, 0x00, 0x00, // "6"
    0x00, 0x00, 0x60, 0x47, 0x48, 0x70, 0x00, 0x00, // "7"
    0x00, 0x00, 0x36, 0x49, 0x49, 0x36, 0x00, 0x00, // "8"
    0x00, 0x00, 0x32, 0x49, 0x49, 0x3E, 0x00, 0x00, // "9"
    0x00, 0x00, 0x3F, 0x44, 0x44, 0x3F, 0x00, 0x00, // "A"
    0x00, 0x00, 0x7F, 0x49, 0x49, 0x36, 0x00, 0x00, // "B"
    0x00, 0x00, 0x3E, 0x41, 0x41, 0x22, 0x00, 0x00, // "C"
    0x00, 0x00, 0x7F, 0x41, 0x41, 0x3E, 0x00, 0x00, // "D"
    0x00, 0x00, 0x7F, 0x49, 0x49, 0x41, 0x00, 0x00, // "E"
    0x00, 0x00, 0x7F, 0x48, 0x48, 0x40, 0x00, 0x00, // "F"
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // " "
};

// Function to shift out a byte of data to the shift register
void customShiftOut(int dataPin, int clockPin, int bitOrder, int value) {
    for (int i = 0; i < 8; i++) {              // Loop through each bit
        digitalWrite(clockPin, LOW);           // Set clock pin LOW
        if (bitOrder == MSBFIRST) {            // If MSB first
            digitalWrite(dataPin, ((0x80 & (value << i)) == 0x80) ? HIGH : LOW); // Write bit to data pin
        } else if (bitOrder == LSBFIRST) {     // If LSB first
            digitalWrite(dataPin, ((0x01 & (value >> i)) == 0x01) ? HIGH : LOW); // Write bit to data pin
        }
        digitalWrite(clockPin, HIGH);          // Set clock pin HIGH
        delayMicroseconds(10);                 // Small delay
    }
}

// Helper function to display a single frame on the LED matrix
void displayFrame(unsigned char *frameData, int frameSize) {
    unsigned char column = 0x80;               // Initialize column bitmask
    for (int i = 0; i < frameSize; i++) {
        digitalWrite(LATCH_PIN, LOW);          // Set latch pin LOW
        customShiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, frameData[i]); // Shift out row data
        customShiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, ~column);      // Shift out column data
        digitalWrite(LATCH_PIN, HIGH);         // Output data from both shift registers
        column >>= 1;                          // Move to the next column
        delay(1);                              // Small delay
    }
}

// Setup function to initialize the GPIO pins
void setup() {
    wiringPiSetup();                           // Initialize wiringPi library
    pinMode(DATA_PIN, OUTPUT);                 // Set data pin as output
    pinMode(LATCH_PIN, OUTPUT);                // Set latch pin as output
    pinMode(CLOCK_PIN, OUTPUT);                // Set clock pin as output
}

// Loop function to display the smiley face and hex characters
void loop() {
    for (int j = 0; j < 500; j++) {            // Repeat to display the smiley face for a period of time
        displayFrame(smileyFace, sizeof(smileyFace)); // Display smiley face
    }

    for (int k = 0; k < sizeof(hexChars) - 8; k++) { // Loop through hex character data
        for (int j = 0; j < 20; j++) {         // Repeat display for each frame
            displayFrame(&hexChars[k], 8);     // Display hex characters
        }
    }
}

// Main function to start the setup and loop functions
int main(void) {                               
    setup();                                   // Initialize pins and setup the system
    while (true) {                             // Infinite loop
        loop();                                // Continuously update display
    }
    return 0;                                  // Return 0 (never reached)
}

