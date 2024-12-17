#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "hardware/i2c.h"
#include "pico/stdlib.h"

void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(int line, int position);
void lcd_string(const char *s);

#endif // LCD_H