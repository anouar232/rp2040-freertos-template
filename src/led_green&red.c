#include "led_green&red.h"

// Initialize the buzzer
void init_leds() {
    // Set the BUZZER_PIN as an output pin
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    // Turn the buzzer off by default
    gpio_put(BUZZER_PIN, 0);
    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);
    gpio_put(GREEN_LED_PIN, 1);
}

// Turn off the buzzer
void putoff_leds() {
    gpio_put(BUZZER_PIN, 0);
    gpio_put(GREEN_LED_PIN, 1);
}

// Turn on the led
void puton_leds() {
    gpio_put(BUZZER_PIN, 1);
    gpio_put( GREEN_LED_PIN, 0);
}
