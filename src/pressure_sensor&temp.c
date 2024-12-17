#include"pressure_sensor&temp.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"


#define BME180_CTRL_MEAS 0xF4
#define BME180_DATA 0xF7

void init_bme180_sensor() {
    // Initialize I2C
    i2c_init(I2C_PORT, 400 * 1000);  // 400kHz I2C speed

    // Set SDA and SCL pins (GPIO 12 and 13)
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);  // SDA on GPIO 12
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);  // SCL on GPIO 13

    // Enable pull-up resistors on SDA and SCL lines
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

void read_bme180_data(int *temperature, int *pressure, int *humidity) {
    uint8_t data[8];

    // Request measurement from BME180 sensor
    uint8_t cmd[2] = {BME180_CTRL_MEAS, 0x27};  // Temperature and Pressure measurement command
    i2c_write_blocking(I2C_PORT, BME180_I2C_ADDR, cmd, 2, false);
    sleep_ms(50); // wait for the sensor to measure data

    // Read 8 bytes of data from the sensor
    i2c_read_blocking(I2C_PORT, BME180_I2C_ADDR, data, 8, false);
   

    // Process the data (example: temperature in Celsius)
    *temperature = (data[0] << 8) | data[1];
    *pressure = (data[2] << 8) | data[3];  // Just an example
    *humidity = data[4];  // Example, might need more processing
}
