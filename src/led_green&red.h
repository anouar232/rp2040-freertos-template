
#include "pico/stdlib.h"


#ifndef BUZZER_H
#define BUZZER_H



#define BUZZER_PIN 2
#define GREEN_LED_PIN 3

void init_leds();
void putoff_leds();
void puton_leds();

#endif // BUZZER_H