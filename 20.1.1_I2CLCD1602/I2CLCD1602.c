/*
Filename    : I2CLCD1602.c
Description : Use the LCD to display time and CPU temp
Author      : Philippe Jos
Modified    : 13/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/20.1.1_I2CLCD1602
*/
#include <stdlib.h>                            // Include standard library
#include <stdio.h>                             // Include standard I/O library
#include <wiringPi.h>                          // Include wiringPi library for GPIO
#include <wiringPiI2C.h>                       // Include wiringPi I2C library
#include <pcf8574.h>                           // Include PCF8574 library for I2C IO expander
#include <lcd.h>                               // Include LCD library
#include <time.h>                              // Include time library

#define BASE 64                                // Base number for PCF8574 pins

// Define the output pins of the PCF8574, connected to the LCD1602 pins
#define RS  BASE + 0                           // Register Select pin
#define RW  BASE + 1                           // Read/Write pin
#define EN  BASE + 2                           // Enable pin
#define LED BASE + 3                           // Backlight control pin
#define D4  BASE + 4                           // Data pin 4
#define D5  BASE + 5                           // Data pin 5
#define D6  BASE + 6                           // Data pin 6
#define D7  BASE + 7                           // Data pin 7

int i2cAddress = 0x3F;                         // Default I2C address for PCF8574 (Serial to Parallel IC Chip)
int lcdHandle;                                 // Handle for the LCD1602

// Function to print CPU temperature on the LCD
void printCpuTemperature() {
    FILE *fp;                                  // File pointer for reading temperature file
    char tempStr[15];                          // String to store temperature reading
    float cpuTemp;                             // Variable to store converted temperature

    fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r"); // Open temperature file
    fgets(tempStr, 15, fp);                    // Read temperature file
    cpuTemp = atof(tempStr) / 1000.0;          // Convert to Celsius degrees
    printf("CPU Temperature: %.2f°C\n", cpuTemp); // Print temperature to console
    lcdPosition(lcdHandle, 0, 0);              // Set LCD cursor position to (0,0)
    lcdPrintf(lcdHandle, "CPU: %.2fC", cpuTemp); // Display CPU temperature on LCD
    fclose(fp);                                // Close file
}

// Function to print system time on the LCD
void printDateTime() {
    time_t rawTime;                            // Raw time variable
    struct tm *timeInfo;                       // Structure to hold local time

    time(&rawTime);                            // Get system time
    timeInfo = localtime(&rawTime);            // Convert to local time
    printf("Current Time: %s", asctime(timeInfo)); // Print time to console
    lcdPosition(lcdHandle, 0, 1);              // Set LCD cursor position to (0,1)
    lcdPrintf(lcdHandle, "Time: %02d:%02d:%02d", // Display system time on LCD
              timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
}

// Function to detect I2C devices
int detectI2CDevice(int address) {
    int fd = wiringPiI2CSetup(address);        // Setup I2C with given address
    if (fd < 0) {                              // Check if setup was successful
        printf("Error: Cannot find device at address 0x%x\n", address);
        return 0;
    } else {
        if (wiringPiI2CWrite(fd, 0) < 0) {     // Check if write was successful
            printf("No device found at address 0x%x\n", address);
            return 0;
        } else {
            printf("Device found at address 0x%x\n", address);
            return 1;
        }
    }
}

// Function to initialize the PCF8574
void initPCF8574() {
    pcf8574Setup(BASE, i2cAddress);            // Initialize PCF8574 with base address
    for (int i = 0; i < 8; i++) {
        pinMode(BASE + i, OUTPUT);             // Set PCF8574 pins to output mode
    }
    digitalWrite(LED, HIGH);                   // Turn on LCD backlight
    digitalWrite(RW, LOW);                     // Set LCD to write mode
}

// Function to initialize the LCD
void initLCD() {
    lcdHandle = lcdInit(2, 16, 4, RS, EN, D4, D5, D6, D7, 0, 0, 0, 0); // Initialize LCD
    if (lcdHandle == -1) {                     // Check if LCD initialization was successful
        printf("lcdInit failed!\n");
        exit(1);
    }
}

// Setup function to initialize the GPIO and LCD
void setup() {
    wiringPiSetup();                           // Initialize wiringPi library
    if (detectI2CDevice(0x3F)) {               // Check for device at address 0x3F
        i2cAddress = 0x3F;
    } else if (detectI2CDevice(0x27)) {        // Check for device at address 0x27
        i2cAddress = 0x27;
    } else {
        printf("No valid I2C address found.\n"
               "Please use 'i2cdetect -y 1' to check the I2C address!\n"
               "Program exiting.\n");
        exit(1);
    }

    initPCF8574();                             // Initialize PCF8574
    initLCD();                                 // Initialize LCD
}

// Loop function to continuously update the LCD
void loop() {
    printCpuTemperature();                     // Print CPU temperature to LCD
    printDateTime();                           // Print system time to LCD
    delay(1000);                               // Wait for 1 second
}

// Main function to start the setup and loop functions
int main(void) {
    setup();                                   // Initialize pins and setup the system
    while (true) {                             // Infinite loop
        loop();                                // Continuously update display
    }
    return 0;                                  // Return 0 (never reached)
}

