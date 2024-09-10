/*
Filename    : MatrixKeypad.cpp
Description : Obtain the key code of 4x4 Matrix Keypad
Author      : Philippe Jos
Modified    : 13/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/22.1.1_MatrixKeypad
*/
#include "Keypad.hpp"                                           // Include Keypad library
#include <stdio.h>                                              // Include standard I/O library

const byte ROWS = 4;                                            // Number of rows on the keypad
const byte COLS = 4;                                            // Number of columns on the keypad

// Key mapping array
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Define the row and column pins for the keypad
byte rowPins[ROWS] = {1, 4, 5, 6};                              // Pins connected to the rows of the keypad
byte colPins[COLS] = {12, 3, 2, 0};                             // Pins connected to the columns of the keypad

// Create Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
    wiringPiSetup();                                            // Initialize wiringPi library
    keypad.setDebounceTime(50);                                 // Set debounce time for key press
}

void loop() {
    char key = keypad.getKey();                                 // Get the state of keys
    if (key) {                                                  // If a key is pressed, print out its key code
        printf("Key: %c \n", key);
    }
}

int main() {
    setup();                                                    // Call the setup function
    while (1) {
        loop();                                                 // Continuously call the loop function
    }
    return 0;                                                   // This line will never be reached
}



