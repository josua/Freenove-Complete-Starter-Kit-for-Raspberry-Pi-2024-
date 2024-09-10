/*
Filename    : ColorfulSoftlight.cpp
Description : Use potentiometers to control RGB LED
Author      : Philippe Jos
Modified    : 07/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/09.1.1_ColorfulSoftlight
*/

#include <wiringPi.h>                                    // Include WiringPi library for GPIO control
#include <stdio.h>                                       // Include standard I/O library
#include <softPwm.h>                                     // Include softPwm library for PWM control
#include <ADCDevice.hpp>                                 // Include ADCDevice header

#define LED_RED_PIN 0                                    // Define pin for Red LED
#define LED_GREEN_PIN 2                                  // Define pin for Green LED
#define LED_BLUE_PIN 3                                   // Define pin for Blue LED
#define THRESHOLD 3                                      // Threshold value to handle noise
#define ALPHA 0.1                                        // Smoothing factor for the low-pass filter

// Pre-computed ratios
const float PWM_RATIO = 100.0 / 255.0;                   // Ratio for mapping ADC value to PWM duty cycle
const float VOLTAGE_RATIO = 3.3 / 255.0;                 // Ratio for mapping ADC value to voltage

ADCDevice *adc;                                          // Define an ADC Device class object

// Struct for color values
struct Color {
    float raw;                                           // Raw ADC value
    float smooth;                                        // Smoothed ADC value
    int pwm;                                             // PWM duty cycle value
    float display;                                       // Display value for debugging
    float voltage;                                       // Calculated voltage
};

// Global variables for RGB values
Color red = {0, 0, 0, 0, 0};
Color green = {0, 0, 0, 0, 0};
Color blue = {0, 0, 0, 0, 0};

// Setup function for initializing ADC device and PWM pins
int setup() {
    adc = new ADCDevice();                               // Initialize ADCDevice object
    
    if (adc->detectI2C(0x48)) {                          // Detect the pcf8591
        delete adc;                                      // Free previously allocated memory
        adc = new PCF8591();                             // Create an instance of PCF8591 if detected
    } else if (adc->detectI2C(0x4b)) {                   // Detect the ads7830
        delete adc;                                      // Free previously allocated memory
        adc = new ADS7830();                             // Create an instance of ADS7830 if detected
    } else {
        printf("No correct I2C address found, \n"
               "Please use command 'i2cdetect -y 1' to check the I2C address! \n"
               "Program Exit. \n");
        return -1;                                       // Exit program if no valid I2C address is found
    }

    wiringPiSetup();                                     // Initialize WiringPi
    
    // Create 3 PWM output pins for RGB LED
    softPwmCreate(LED_RED_PIN, 0, 100);                  // Initialize PWM for Red LED
    softPwmCreate(LED_GREEN_PIN, 0, 100);                // Initialize PWM for Green LED
    softPwmCreate(LED_BLUE_PIN, 0, 100);                 // Initialize PWM for Blue LED

    return 0;                                            // Return 0 to indicate success
}

// Update and apply the low-pass filter to the color values
void updateColor(Color &color, int rawValue) {
    color.raw = rawValue;                                // Update the raw value
    color.smooth = ALPHA * color.raw + (1 - ALPHA) * color.smooth; // Apply the low-pass filter
    color.pwm = static_cast<int>(color.smooth * PWM_RATIO); // Map the smoothed value to PWM range
    
    // Apply threshold to handle noise near 0 and set values near 255 to 255
    if (color.pwm < THRESHOLD) {
        color.pwm = 0;
    } else if (color.pwm > (100 - THRESHOLD)) {
        color.pwm = 100;
    }

    color.display = 255 - (color.pwm * 255.0 / 100.0);   // Update the display value
    color.voltage = color.display * VOLTAGE_RATIO;       // Calculate the voltage
}

// Loop function to read analog values and map them to PWM values for RGB LED
void loop() {
    // Read analog values of 3 potentiometers
    updateColor(red, adc->analogRead(0));
    updateColor(green, adc->analogRead(1));
    updateColor(blue, adc->analogRead(2));

    // Output PWM values
    softPwmWrite(LED_RED_PIN, red.pwm);
    softPwmWrite(LED_GREEN_PIN, green.pwm);
    softPwmWrite(LED_BLUE_PIN, blue.pwm);

    // Print out the ADC values and voltage values
    printf("ADC Red: %.2f \tGreen: %.2f \tBlue: %.2f\n", 
           red.display, 
           green.display, 
           blue.display);
    printf("VCC Red: %.2fV\tGreen: %.2fV\tBlue: %.2fV\n", 
           red.voltage, 
           green.voltage, 
           blue.voltage);

    delay(1);                                           // Small delay to improve responsiveness
}

// Main function to set up the environment and enter the main loop
int main(void) {
    if (setup() == -1) return -1;                       // Call setup function, exit if it returns -1
    while (true) loop();                                // Loop indefinitely
    return 0;                                           // This line will never be reached
}

