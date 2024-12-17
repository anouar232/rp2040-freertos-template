#include "pico/stdlib.h"
#include "hardware/i2c.h"

#ifndef BME180_SENSOR_H
#define BME180_SENSOR_H

// Set the I2C address based on your sensor's configuration
#define BME180_I2C_ADDR 0x77  // or 0x76 depending on your sensor configuration

#define I2C_PORT i2c1
#define SDA_PIN 14 // GPIO 12 for SDA
#define SCL_PIN 15  // GPIO 13 for SCL

void init_bme180_sensor();
void read_bme180_data(int *temperature, int *pressure, int *humidity);

#endif // BME180_SENSOR_H
