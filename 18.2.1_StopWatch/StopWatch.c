/*
Filename    : StopWatch.c
Description : 4 Digit 7 Segment Display controlled by 74HC595
Author      : Philippe Jos
Modified    : 12/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/18.2.1_StopWatch
*/
#include <stdio.h>                             // Include standard I/O library
#include <wiringPi.h>                          // Include wiringPi library for GPIO
#include <wiringShift.h>                       // Include wiringShift library for shift register functions
#include <signal.h>                            // Include signal library for handling signals
#include <unistd.h>                            // Include Unix standard library for sleep functions

// Define pins connected to the 74HC595 shift register
#define DATA_PIN 5                             // DS Pin of 74HC595 (Pin 14)
#define LATCH_PIN 4                            // ST_CP Pin of 74HC595 (Pin 12)
#define CLOCK_PIN 1                            // CH_CP Pin of 74HC595 (Pin 11)

// Define common pins for the 7-segment display
const int SEGMENT_PINS[] = {0, 2, 3, 12};      // Pins for 7-segment display

// Character codes for digits 0-9 on a common anode 7-segment display
unsigned char DIGIT_CODES[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

int counter = 0;                               // Variable to hold the number to be displayed

// Function to select which digit to display on the 7-segment display
void selectDigit(int digit) {                  
    digitalWrite(SEGMENT_PINS[0], ((digit & 0x08) == 0x08) ? LOW : HIGH); // Select thousands digit
    digitalWrite(SEGMENT_PINS[1], ((digit & 0x04) == 0x04) ? LOW : HIGH); // Select hundreds digit
    digitalWrite(SEGMENT_PINS[2], ((digit & 0x02) == 0x02) ? LOW : HIGH); // Select tens digit
    digitalWrite(SEGMENT_PINS[3], ((digit & 0x01) == 0x01) ? LOW : HIGH); // Select units digit
}

// Function to shift out a byte of data to the shift register
void shiftOut(int dataPin, int clockPin, int bitOrder, int value) {
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

// Function to output data to the 74HC595 shift register
void outputData(int8_t data) {                 
    digitalWrite(LATCH_PIN, LOW);              // Set latch pin LOW
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data); // Shift out data
    digitalWrite(LATCH_PIN, HIGH);             // Set latch pin HIGH
}

// Helper function to display a single digit
void displayDigit(int digit, int value) {
    outputData(0xff);                          // Clear display
    selectDigit(digit);                        // Select the specific digit
    outputData(DIGIT_CODES[value]);            // Output the digit value
    delay(1);                                  // Short delay
}

// Function to display a number on the 7-segment display
void displayNumber(int number) {               
    displayDigit(0x01, number % 10);           // Display units digit
    displayDigit(0x02, (number / 10) % 10);    // Display tens digit
    displayDigit(0x04, (number / 100) % 10);   // Display hundreds digit
    displayDigit(0x08, (number / 1000) % 10);  // Display thousands digit
}

// Timer function to update the counter
void timerHandler(int signal) {                
    if (signal == SIGALRM) {                   // If signal is SIGALRM
        counter++;                             // Increment counter
        alarm(1);                              // Set next timer to 1 second
        printf("%d \n", counter);              // Print counter value
    }
}

// Setup function to initialize the GPIO pins
void setup() {                                
    wiringPiSetup();                           // Initialize wiringPi library
    pinMode(DATA_PIN, OUTPUT);                 // Set data pin as output
    pinMode(LATCH_PIN, OUTPUT);                // Set latch pin as output
    pinMode(CLOCK_PIN, OUTPUT);                // Set clock pin as output
    for (int i = 0; i < 4; i++) {              // Loop through segment pins
        pinMode(SEGMENT_PINS[i], OUTPUT);      // Set segment pins as output
        digitalWrite(SEGMENT_PINS[i], HIGH);   // Initialize segment pins to HIGH
    }
    signal(SIGALRM, timerHandler);             // Configure the timer handler
    alarm(1);                                  // Set the timer to 1 second
}

// Loop function to continuously display the counter
void loop() {                                  
    displayNumber(counter);                    // Display the counter value
}

// Main function to start the setup and loop functions
int main(void) {                               
    setup();                                   // Initialize pins and setup the system
    while (true) {                             // Infinite loop
        loop();                                // Continuously update display
    }
    return 0;                                  // Return 0 (never reached)
}
