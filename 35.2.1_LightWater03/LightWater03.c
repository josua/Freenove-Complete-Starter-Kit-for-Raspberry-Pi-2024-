/*
Filename    : LightWater03.c
Description : Control LED by 74HC595 on DIY circuit board
Author      : Philippe Jos
Modified    : 18/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/35.2.1_LightWater03
*/
#include <stdio.h>                                               // Include standard I/O library
#include <unistd.h>                                              // Include Unix standard library
#include <wiringPi.h>                                            // Include wiringPi library
#include <wiringShift.h>                                         // Include wiringShift library

#define DATA_PIN    0                                            // DS Pin of 74HC595 (Pin 14)
#define LATCH_PIN   2                                            // ST_CP Pin of 74HC595 (Pin 12)
#define CLOCK_PIN   3                                            // SH_CP Pin of 74HC595 (Pin 11)

// Define an array to store the pulse width of LED, which will be output to the 8 LEDs in order
const int PULSE_WIDTH[] = {0, 0, 0, 0, 0, 0, 0, 0, 64, 32, 16, 8, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0};


// Function to output data to the shift register
void outputData(int8_t data) {
    digitalWrite(LATCH_PIN, LOW);                               // Set latch pin low
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, data);              // Shift out data
    digitalWrite(LATCH_PIN, HIGH);                              // Set latch pin high
}

// Function to map a value from one range to another
long mapValue(long value, long fromLow, long fromHigh, long toLow, long toHigh) {
    return (toHigh - toLow) * (value - fromLow) / (fromHigh - fromLow) + toLow;
}

// Setup function to initialize the pins
void setup() {
    wiringPiSetup();                                            // Initialize wiringPi
    pinMode(DATA_PIN, OUTPUT);                                  // Set data pin as output
    pinMode(LATCH_PIN, OUTPUT);                                 // Set latch pin as output
    pinMode(CLOCK_PIN, OUTPUT);                                 // Set clock pin as output
}

// Loop function to control the LED pattern
void loop() {
    static int index = 0;                                       // Static variable to keep track of the current position in PULSE_WIDTH array
    static long lastMove = millis();                            // Static variable to record the last move time
    int moveSpeed = 100;                                        // Variable for controlling move speed
    
    if (millis() - lastMove > moveSpeed) {                      // Speed control
        lastMove = millis();                                    // Record the time point of the move
        index++;                                                // Move to the next index
        if (index > 15) index = 0;                              // Reset index if it exceeds 15
    }
    
    for (int i = 0; i < 64; i++) {                              // The cycle of PWM is 64 cycles
        int8_t data = 0;
        for (int j = 0; j < 8; j++) {                           // Calculate the output state
            if (i < PULSE_WIDTH[index + j]) {                   // Calculate the LED state according to the pulse width
                data |= 0x01 << j;                              // Calculate the data
            }
        }
        outputData(data);                                       // Send the data to 74HC595
    }
}

// Main function to start the setup and loop functions
int main(void) {
    setup();                                                    // Initialize pins and setup configuration
    while (true) {
        loop();                                                 // Continuously run the loop function
    }
    return 0;                                                   // Return 0 (never reached)
}

