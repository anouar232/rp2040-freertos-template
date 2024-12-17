#include "buzzer_pwm.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

void buzzer_init(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_enabled(slice_num, true);
}

void buzzer_set_tone(uint pin, uint frequency, uint volume) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint sys_clk = clock_get_hz(clk_sys);
    float divider = sys_clk / (frequency * 255);
    pwm_set_clkdiv(slice_num, divider);
    pwm_set_wrap(slice_num, 255);
    pwm_set_gpio_level(pin, volume);  // Use volume to set PWM level
}

void buzzer_off(uint pin) {
    pwm_set_gpio_level(pin, 0);
}

