#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "led_green&red.h"
#include "vibarator_sensor.h"
#include "pressure_sensor&temp.h"
#include "buzzer_pwm.h"
#include "lcd.h"

#define INITIAL_FREQUENCY 2000
#define MAX_VOLUME 255
#define VOLUME_INCREMENT 6
#define VOLUME_INCREASE_INTERVAL_MS 1000

bool flag = true;
bool normal_pressure=false;
static uint current_volume = 0;
static uint64_t last_volume_increase_time = 0;
static int temperature, pressure, humidity = 0;

void lcd_task(void *pvParameters)
{
    while (true) {
        if (flag == true) {
            lcd_clear();
            lcd_set_cursor(0, 0);
            char temp_str[20];
            snprintf(temp_str, sizeof(temp_str), "Temperature: %d", temperature);
            lcd_string(temp_str);
            
            lcd_set_cursor(1, 0);
            char pressure_str[20];
            snprintf(pressure_str, sizeof(pressure_str), "Pressure: %d", pressure);
            lcd_string(pressure_str);
        }
        else if(flag==false && normal_pressure==true)
        {
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_string("Normal pressure");
            lcd_set_cursor(1, 0);
            lcd_string("Returned!");
            flag=true;
        }
        else{
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_string("Low pressure");
            lcd_set_cursor(1, 0);
            lcd_string("detected!");
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
    }
}

void sensor_task(void *pvParameters)
{
    while (true) {
        read_bme180_data(&temperature, &pressure, &humidity);

        if (pressure < 3000) {
            flag = false;
            normal_pressure=false;
          
        } else {
            if (flag == false) {              
              
                normal_pressure=true;
            }
            printf("Normal pressure, deactivating buzzer\n");
          // Reset volume when pressure returns to normal
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS); // Delay for 1 second
    }
}

void buzzer_task(void *pvParameters)
{
    while (true) {
        uint64_t current_time = time_us_64() / 1000;

        if (pressure < 3000 ||temperature>800) {
            if (current_time - last_volume_increase_time >= VOLUME_INCREASE_INTERVAL_MS) {
                if (current_volume < MAX_VOLUME) {
                    current_volume += VOLUME_INCREMENT;
                    if (current_volume > MAX_VOLUME) {
                        current_volume = MAX_VOLUME;
                    }
                }
                last_volume_increase_time = current_time;
            }

            buzzer_set_tone(BUZZER_PIN_PWM, INITIAL_FREQUENCY, current_volume);
            printf("Current buzzer volume: %u\n", current_volume);
        } else {
            buzzer_off(BUZZER_PIN_PWM);
           
            current_volume = 0;  
        }

        vTaskDelay(100 / portTICK_PERIOD_MS); // Delay for 100 ms to give time for other tasks
    }
}

void led_task(void *pvParameters)
{
    while (true) {
        if (flag == false) {
            puton_leds(); // Turn on red LED when low pressure detected
        } else {
            putoff_leds(); // Turn off LED when normal pressure is detected
        }

        vTaskDelay(100 / portTICK_PERIOD_MS); // Delay for 100 ms to give time for other tasks
    }
}

int main()
{
    stdio_init_all();

    // Initialize hardware
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
    lcd_init();  // Initialize LCD
    init_leds();  // Initialize LEDs
    buzzer_init(BUZZER_PIN_PWM);  // Initialize Buzzer
    init_bme180_sensor();  // Initialize BME180 sensor

    // Create FreeRTOS tasks
    xTaskCreate(lcd_task, "LCD_Task", 256, NULL, 2, NULL);
    xTaskCreate(sensor_task, "Sensor_Task", 256, NULL, 1, NULL);
    xTaskCreate(buzzer_task, "Buzzer_Task", 256, NULL, 1, NULL);
    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    while (1) {
        // The main function should never return, as FreeRTOS takes over
    }

    return 0;
}
