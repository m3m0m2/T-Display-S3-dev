/*
 * Copyright (C) 2023, Mauro Meneghin
 */

#ifndef INCLUDE_UI_H
#define INCLUDE_UI_H

#include "adc.h"

#include <lvgl.h>

typedef struct
{
    lv_obj_t* scr;
    adc_t* adc0;
} ui_data_t;

void ui_create(ui_data_t* ui_data);
void ui_input_handler(uint32_t pin, int value);

#endif
