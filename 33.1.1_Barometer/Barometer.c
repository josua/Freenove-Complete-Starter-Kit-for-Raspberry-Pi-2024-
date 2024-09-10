/*
Filename    : Barometer.c
Description : Read atmospheric pressure, temperature and current altitude of BMP180 pressure sensor.
Author      : Philippe Jos
Modified    : 16/07/2024
Reference   : https://github.com/Freenove/Freenove_Complete_Starter_Kit_for_Raspberry_Pi/tree/main/Code/C_Code/33.1.1_Barometer
*/
#include <stdio.h>                                                          // Include standard I/O library
#include <stdint.h>                                                         // Include standard integer types
#include <fcntl.h>                                                          // Include file control options
#include <stdlib.h>                                                         // Include standard library
#include <unistd.h>                                                         // Include standard symbolic constants and types
#include <linux/i2c-dev.h>                                                  // Include I2C device definitions
#include <linux/i2c.h>                                                      // Include I2C protocol definitions
#include <sys/ioctl.h>                                                      // Include I/O control operations
#include "smbus.h"                                                          // Include SMBus functions
#include <math.h>                                                           // Include math library

#define BMP180_I2C_ADDRESS 0x77                                             // Define BMP180 I2C address
const unsigned char BMP180_OVERSAMPLING_SETTING = 3;                        // Define oversampling setting for BMP180
const float SEA_LEVEL_PRESSURE = 1013.25;                                   // Sea-level pressure in hPa

// Calibration values - These are stored in the BMP180
short int ac1, ac2, ac3, b1, b2, mb, mc, md;                                // Declare calibration variables
unsigned short int ac4, ac5, ac6;                                           // Declare more calibration variables
int b5;                                                                     // Declare b5 variable
unsigned int temperature, pressure, altitude;                               // Declare temperature, pressure, and altitude variables

// Structure to hold calibration data addresses and pointers
struct CalibrationData {
    __u8 address;
    short int* ptr;
};

// Array of calibration data
CalibrationData calibration_data[] = {
    {0xAA, &ac1},
    {0xAC, &ac2},
    {0xAE, &ac3},
    {0xB0, (short int*)&ac4},  // Cast to short int* for unsigned short int
    {0xB2, (short int*)&ac5},  // Cast to short int* for unsigned short int
    {0xB4, (short int*)&ac6},  // Cast to short int* for unsigned short int
    {0xB6, &b1},
    {0xB8, &b2},
    {0xBA, &mb},
    {0xBC, &mc},
    {0xBE, &md}
};

// Function to initialize I2C communication with BMP180 sensor
int bmp180_i2c_Begin() {
    int fd;                                                                 // File descriptor for I2C device
    const char *fileName = "/dev/i2c-1";                                    // I2C device file
    if ((fd = open(fileName, O_RDWR)) < 0) {                                // Open I2C device for reading and writing
        perror("Failed to open the i2c bus");                               // Print error message
        exit(1);                                                            // Exit program if error
    }
    if (ioctl(fd, I2C_SLAVE, BMP180_I2C_ADDRESS) < 0) {                     // Set I2C device address
        perror("Failed to acquire bus access and/or talk to slave");        // Print error message
        close(fd);                                                          // Close I2C device
        exit(1);                                                            // Exit program if error
    }
    return fd;                                                              // Return file descriptor
}

// Function to read two bytes from the BMP180 sensor and return as a 16-bit integer
__s32 bmp180_i2c_Read_Int(int fd, __u8 address) {
    __s32 res = i2c_smbus_read_word_data(fd, address);                      // Read word data from I2C device
    if (res < 0) {                                                          // Check for error
        perror("Failed to read word data");                                 // Print error message
        close(fd);                                                          // Close I2C device
        exit(1);                                                            // Exit program if error
    }
    res = ((res << 8) & 0xFF00) | ((res >> 8) & 0xFF);                      // Swap bytes
    return res;                                                             // Return 16-bit integer
}

// Function to write a byte to the BMP180 sensor
void bmp180_i2c_Write_Byte(int fd, __u8 address, __u8 value) {
    if (i2c_smbus_write_byte_data(fd, address, value) < 0) {                // Write byte data to I2C device
        perror("Failed to write byte data");                                // Print error message
        close(fd);                                                          // Close I2C device
        exit(1);                                                            // Exit program if error
    }
}

// Function to read a block of data from the BMP180 sensor
void bmp180_i2c_Read_Block(int fd, __u8 address, __u8 length, __u8 *values) {
    if (i2c_smbus_read_i2c_block_data(fd, address, length, values) < 0) {   // Read block data from I2C device
        perror("Failed to read block data");                                // Print error message
        close(fd);                                                          // Close I2C device
        exit(1);                                                            // Exit program if error
    }
}

// Function to read calibration data from the BMP180 sensor
void bmp180_Calibration() {
    int fd = bmp180_i2c_Begin();                                            // Initialize I2C communication

    // Iterate through the list and read calibration data
    for (int i = 0; i < sizeof(calibration_data)/sizeof(calibration_data[0]); i++) {
        *calibration_data[i].ptr = bmp180_i2c_Read_Int(fd, calibration_data[i].address);
    }

    close(fd);                                                              // Close I2C device
}

// Function to read the uncompensated temperature value from the BMP180 sensor
unsigned int bmp180_ReadUT() {
    int fd = bmp180_i2c_Begin();                                            // Initialize I2C communication
    bmp180_i2c_Write_Byte(fd, 0xF4, 0x2E);                                  // Request temperature reading
    usleep(5000);                                                           // Wait for measurement
    unsigned int ut = bmp180_i2c_Read_Int(fd, 0xF6);                        // Read uncompensated temperature value
    close(fd);                                                              // Close I2C device
    return ut;                                                              // Return uncompensated temperature value
}

// Function to read the uncompensated pressure value from the BMP180 sensor
unsigned int bmp180_ReadUP() {
    int fd = bmp180_i2c_Begin();                                            // Initialize I2C communication
    bmp180_i2c_Write_Byte(fd, 0xF4, 0x34 + (BMP180_OVERSAMPLING_SETTING << 6)); // Request pressure reading with oversampling setting
    usleep((2 + (3 << BMP180_OVERSAMPLING_SETTING)) * 1000);                // Wait for measurement
    __u8 values[3];                                                         // Array to store pressure data
    bmp180_i2c_Read_Block(fd, 0xF6, 3, values);                             // Read pressure data block
    close(fd);                                                              // Close I2C device
    unsigned int up = (((unsigned int) values[0] << 16) | ((unsigned int) values[1] << 8) | (unsigned int) values[2]) >> (8 - BMP180_OVERSAMPLING_SETTING); // Combine pressure data
    return up;                                                              // Return uncompensated pressure value
}

// Function to calculate pressure given uncalibrated pressure value
unsigned int bmp180_GetPressure(unsigned int up) {
    int x1, x2, x3, b3, b6, p;                                              // Intermediate calculation variables
    unsigned int b4, b7;                                                    // Intermediate calculation variables
    b6 = b5 - 4000;                                                         // Calculate B6
    x1 = (b2 * (b6 * b6) >> 12) >> 11;                                      // Calculate X1
    x2 = (ac2 * b6) >> 11;                                                  // Calculate X2
    x3 = x1 + x2;                                                           // Calculate X3
    b3 = (((((int) ac1) * 4 + x3) << BMP180_OVERSAMPLING_SETTING) + 2) >> 2; // Calculate B3
    x1 = (ac3 * b6) >> 13;                                                  // Calculate X1
    x2 = (b1 * ((b6 * b6) >> 12)) >> 16;                                    // Calculate X2
    x3 = ((x1 + x2) + 2) >> 2;                                              // Calculate X3
    b4 = (ac4 * (unsigned int) (x3 + 32768)) >> 15;                         // Calculate B4
    b7 = ((unsigned int) (up - b3) * (50000 >> BMP180_OVERSAMPLING_SETTING)); // Calculate B7
    if (b7 < 0x80000000) {                                                  // Calculate pressure P
        p = (b7 << 1) / b4;                                                 // Calculate pressure P
    } else {
        p = (b7 / b4) << 1;                                                 // Calculate pressure P
    }
    x1 = (p >> 8) * (p >> 8);                                               // Calculate X1
    x1 = (x1 * 3038) >> 16;                                                 // Calculate X1
    x2 = (-7357 * p) >> 16;                                                 // Calculate X2
    p += (x1 + x2 + 3791) >> 4;                                             // Calculate pressure P
    return p;                                                               // Return pressure value
}

// Function to calculate temperature given uncalibrated temperature value
unsigned int bmp180_GetTemperature(unsigned int ut) {
    int x1, x2;                                                             // Intermediate calculation variables
    x1 = (((int) ut - (int) ac6) * (int) ac5) >> 15;                        // Calculate X1
    x2 = ((int) mc << 11) / (x1 + md);                                      // Calculate X2
    b5 = x1 + x2;                                                           // Calculate B5
    unsigned int result = ((b5 + 8) >> 4);                                  // Calculate temperature result
    return result;                                                          // Return temperature value
}

// Function to calculate altitude given pressure
float bmp180_GetAltitude(float pressure) {
    float altitude;                                                         // Declare altitude variable
    altitude = 44330.0 * (1.0 - pow(pressure / SEA_LEVEL_PRESSURE, 0.1903)); // Calculate altitude
    altitude = fabs(altitude);                                              // Take the absolute value of altitude
    return altitude;                                                        // Return altitude value
}

void loop() {
    bmp180_Calibration();                                                   // Perform sensor calibration
    temperature = bmp180_GetTemperature(bmp180_ReadUT());                   // Read and calculate temperature
    pressure = bmp180_GetPressure(bmp180_ReadUP());                         // Read and calculate pressure
    float alt = bmp180_GetAltitude((float)pressure / 100.0);                // Calculate altitude from pressure
    altitude = (unsigned int)alt;                                           // Assign altitude to the global variable

    printf("Temperature: %.1f °C\n", ((double)temperature) / 10);           // Print temperature
    printf("Pressure: %.2f hPa\n", ((double)pressure) / 100);               // Print pressure
    printf("Altitude: %.1f m\n", alt);                                      // Print altitude
    printf("-------------------------------------------------\n");          // Print separator line
    usleep(2000 * 1000);                                                    // Wait for 2 seconds
}

int main(void) {
    while (1) {                                                             // Infinite loop to continuously read sensor data
        loop();
    }
    return 0;                                                               // Return 0 (never reached)
}
