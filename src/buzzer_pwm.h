#ifndef BUZZER_PWM_H
#define BUZZER_PWM_H

#include "pico/stdlib.h"


#define BUZZER_PIN_PWM 6

void buzzer_init(uint pin);
void buzzer_set_on(uint pin);
void buzzer_off(uint pin);

#endif // BUZZER_PWM_H

