/*
Filename    : Motor.cpp
Description : Motor controlled by L293D and Potentiometer
Author      : Philippe Jos
Modified    : 09/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/13.1.1_Motor
*/
#include <stdio.h>                                                    // Include standard I/O library
#include <cstdlib>                                                    // Include standard library for exit()
#include <cmath>                                                      // Include cmath for fabs()
#include <memory>                                                     // Include memory management library
#include <wiringPi.h>                                                 // Include WiringPi library
#include <softPwm.h>                                                  // Include soft PWM library
#include <ADCDevice.hpp>                                              // Include ADC device header

#define MOTOR_PIN1    2                                               // Define pins connected to L293D motor driver
#define MOTOR_PIN2    0
#define ENABLE_PIN    3

// Sensor I2C addresses
constexpr int PCF8591_ADDRESS = 0x48;                                 // I2C address for PCF8591
constexpr int ADS7830_ADDRESS = 0x4b;                                 // I2C address for ADS7830

// Threshold for stopping the motor
constexpr int THRESHOLD = 5;                                          // Define a threshold value

// Store an instance of an ADC Device object, as a smart pointer
std::unique_ptr<ADCDevice> adc;                                       // Unique pointer to ADCDevice object

void initADC() {                                                      // Function to initialize ADC
    if (adc->detectI2C(PCF8591_ADDRESS)) {                            // Check for PCF8591 I2C address
        adc = std::make_unique<PCF8591>();                            // Create PCF8591 instance if detected
    } else if (adc->detectI2C(ADS7830_ADDRESS)) {                     // Check for ADS7830 I2C address
        adc = std::make_unique<ADS7830>();                            // Create ADS7830 instance if detected
    } else {                                                          // If no ADC detected
        printf("No correct I2C address found,\n"
               "Please use command 'i2cdetect -y 1' to check the I2C address!\n"
               "Program Exit.\n");                          // Print error message
        exit(EXIT_FAILURE);                                           // Exit program if no ADC detected
    }
}

void setup() {                                                        // Function to set up the program
    adc = std::make_unique<ADCDevice>();                              // Create initial ADCDevice instance
    initADC();                                                        // Initialize ADC

    if (wiringPiSetup() == -1) {                                      // Set up WiringPi and check for failure
        fprintf(stderr, "WiringPi setup failed.\n");
        exit(EXIT_FAILURE);                                           // Exit program if WiringPi setup fails
    }

    pinMode(ENABLE_PIN, OUTPUT);                                      // Set enablePin as output
    pinMode(MOTOR_PIN1, OUTPUT);                                      // Set motorPin1 as output
    pinMode(MOTOR_PIN2, OUTPUT);                                      // Set motorPin2 as output
    softPwmCreate(ENABLE_PIN, 0, 100);                                // Initialize soft PWM on enablePin with 0-100 range
}

// Function to map a value from one range to another
long map(long value, long fromLow, long fromHigh, long toLow, long toHigh) {
    return (toHigh - toLow) * (value - fromLow) / (fromHigh - fromLow) + toLow;
}

// Function to set motor direction and speed
void setMotor(int motorPin1State, int motorPin2State, int pwmValue, const char* direction) {
    digitalWrite(MOTOR_PIN1, motorPin1State);                         // Set motorPin1 state
    digitalWrite(MOTOR_PIN2, motorPin2State);                         // Set motorPin2 state
    softPwmWrite(ENABLE_PIN, pwmValue);                               // Set PWM duty cycle
    printf("%s\n", direction);                                        // Print direction
    printf("The PWM duty cycle is %d%%\n", pwmValue);                 // Print duty cycle
}

// Function to control the motor based on ADC value
void motor(int ADC) {
    int value = ADC - 128;                                            // Center ADC value around 0
    if (abs(value) <= THRESHOLD) {                                    // If value is within the threshold range, stop the motor
        setMotor(LOW, LOW, 0, "Stop");
    } else if (value > 0) {                                           // If value is positive, move forward
        int pwmValue = map(value, THRESHOLD, 127, 0, 100);            // Calculate PWM duty cycle
        setMotor(HIGH, LOW, pwmValue, "Moving Forward");
    } else {                                                          // If value is negative, move backward
        int pwmValue = map(fabs(value), THRESHOLD, 127, 0, 100);      // Calculate PWM duty cycle
        setMotor(LOW, HIGH, pwmValue, "Moving Backward");
    }
}

void loop() {                                                         // Main loop function
    int value = adc->analogRead(0);                                   // Read ADC value from channel 0
    printf("ADC value : %d \n", value);                               // Print ADC value
    motor(value);                                                     // Control motor based on ADC value
    delay(100);                                                       // Delay for 100ms
}

int main(void) {                                                      // Main function
    setup();                                                          // Call setup function
    while (true) {                                                    // Infinite loop
        loop();                                                       // Call loop function
    }
    return 0;                                                         // Return 0 (never reached)
}


