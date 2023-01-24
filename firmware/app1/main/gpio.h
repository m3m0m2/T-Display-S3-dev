/*
 * Copyright (C) 2023, Mauro Meneghin
 */

#ifndef INCLUDE_GPIO_H
#define INCLUDE_GPIO_H

#include <stdint.h>

typedef struct
{
    uint32_t data[2];
} gpio_in_t;

typedef void input_handler_t(uint32_t pin, int value);

void gpio_register_handler(input_handler_t handler);

gpio_in_t gpio_get_state();
int gpio_get_pin(gpio_in_t* state, int pin);
void gpio_set_pin(gpio_in_t* state, int pin, int value);

void gpio_init();

#endif
