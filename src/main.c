// #include "FreeRTOS.h"
// #include "task.h"
// #include "pico/stdlib.h"
// #include "led_green&red.h"
// #include "vibarator_sensor.h"
// #include "pressure_sensor&temp.h"
// #include "buzzer_pwm.h"
// #include "lcd.h"



// bool flag = true;
// bool normal_pressure=false;
// static uint current_volume = 0;
// static uint64_t last_volume_increase_time = 0;
// static int temperature, pressure, humidity = 0;
// bool vibarator_flag=false;

// void lcd_task(void *pvParameters)
// {
//     while (true) {


     
//         if(vibarator_flag==true)
//         {
//            lcd_clear();
//             lcd_set_cursor(0, 0);
//             lcd_string("Vibarator sensor");
//             lcd_set_cursor(1, 0);
//             lcd_string("detected!");
//             vibarator_flag=false;
//             sleep_ms(1000);
//             lcd_clear();
//             lcd_set_cursor(0, 0);
//             lcd_string("Check");
//             lcd_set_cursor(1, 0);
//             lcd_string("Tire!");
//             vibarator_flag=false;
//         }
//        else if (flag == true) {
//             lcd_clear();
//             lcd_set_cursor(0, 0);
//             char temp_str[20];
//             snprintf(temp_str, sizeof(temp_str), "Temperature: %d", temperature);
//             lcd_string(temp_str);
            
//             lcd_set_cursor(1, 0);
//             char pressure_str[20];
//             snprintf(pressure_str, sizeof(pressure_str), "Pressure: %d", pressure);
//             lcd_string(pressure_str);
//         }
//         else if(flag==false && normal_pressure==true)
//         {
//             lcd_clear();
//             lcd_set_cursor(0, 0);
//             lcd_string("Normal pressure");
//             lcd_set_cursor(1, 0);
//             lcd_string("Returned!");
//             flag=true;
//         }
//         else{
//             lcd_clear();
//             lcd_set_cursor(0, 0);
//             lcd_string("Low pressure");
//             lcd_set_cursor(1, 0);
//             lcd_string("detected!");
//         }

//         vTaskDelay(1500 / portTICK_PERIOD_MS); // Delay for 1 second
//     }
// }



// void vibrator_task(void *pvParameters)
// {
 
//      if(get_vibarator_sensor()==1)
     
//             {vibarator_flag=true;}


//         vTaskDelay(100 / portTICK_PERIOD_MS); // Delay for 100 ms to give time for other tasks
    
// }
// void sensor_task(void *pvParameters)
// {
//     while (true) {
//         read_bme180_data(&temperature, &pressure, &humidity);

//         if (pressure < 3000) {
//             flag = false;
//             normal_pressure=false;
          
//         } else {
//             if (flag == false) {              
              
//                 normal_pressure=true;
//             }
//             printf("Normal pressure, deactivating buzzer\n");
//           // Reset volume when pressure returns to normal
//         }

//         vTaskDelay(2000 / portTICK_PERIOD_MS); // Delay for 1 second
//     }
// }

// void buzzer_task(void *pvParameters)
// {
    
//     if(flag==false)
//     {
//         buzzer_set_on(BUZZER_PIN_PWM);
//     }
//     else{
//         buzzer_off(BUZZER_PIN_PWM);
//     }

//         vTaskDelay(100 / portTICK_PERIOD_MS); // Delay for 100 ms to give time for other tasks
//     }



// void led_task(void *pvParameters)
// {
//     while (true) {
//         if (flag == false) {
//             puton_leds(); // Turn on red LED when low pressure detected
//         } else {
//             putoff_leds(); // Turn off LED when normal pressure is detected
//         }

//         vTaskDelay(100 / portTICK_PERIOD_MS); // Delay for 100 ms to give time for other tasks
//     }
// }

// int main()
// {
//     stdio_init_all();

//     // Initialize hardware
//     i2c_init(i2c_default, 100 * 1000);
//     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
//     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
//     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
//     lcd_init();  // Initialize LCD
//     init_leds();  // Initialize LEDs
//     buzzer_init(BUZZER_PIN_PWM);  // Initialize Buzzer
//     init_bme180_sensor();  // Initialize BME180 sensor
//     //init_vibarator_sensor();
//     // Create FreeRTOS tasks
//     xTaskCreate(lcd_task, "LCD_Task", 256, NULL, 2, NULL);
//     xTaskCreate(sensor_task, "Sensor_Task", 256, NULL, 1, NULL);
//     xTaskCreate(buzzer_task, "Buzzer_Task", 256, NULL, 1, NULL);
//     xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
//     //xTaskCreate(vibrator_task, "Vibrator_Task", 256, NULL, 1, NULL);

//     // Start the FreeRTOS scheduler
//     vTaskStartScheduler();

//     while (1) {
//         // The main function should never return, as FreeRTOS takes over
//     }

//     return 0;
// }

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
bool vibarator_flag=false;


void lcd_task(void *pvParameters)
{
    while (true) {
                if(vibarator_flag==true)
        {
           lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_string("Vibarator sensor");
            lcd_set_cursor(1, 0);
            lcd_string("detected!");
            vibarator_flag=false;
            sleep_ms(1000);
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_string("Check");
            lcd_set_cursor(1, 0);
            lcd_string("Tire!");
            vibarator_flag=false;
        }
        else if (flag == true) {
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
    while(true)
    {
if(flag==false)
    {
        buzzer_set_on(BUZZER_PIN_PWM);
    }
    else{
        buzzer_off(BUZZER_PIN_PWM);
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

void vibrator_task(void *pvParameters)
{
 
 while(true)
 {
 if(get_vibarator_sensor()==1)
     
            {vibarator_flag=true;}


        vTaskDelay(100 / portTICK_PERIOD_MS);
 }
     // Delay for 100 ms to give time for other tasks
    
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
    init_vibarator_sensor();
 
    xTaskCreate(lcd_task, "LCD_Task", 256, NULL, 2, NULL);
    xTaskCreate(sensor_task, "Sensor_Task", 256, NULL, 1, NULL);
    xTaskCreate(buzzer_task, "Buzzer_Task", 256, NULL, 1, NULL);
    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
        xTaskCreate(vibrator_task, "vibrator_task", 256, NULL, 1, NULL);


    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    while (1) {
        // The main function should never return, as FreeRTOS takes over
    }

    return 0;
}