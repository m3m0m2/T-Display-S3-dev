/*
 * Copyright (C) 2023, Mauro Meneghin
 */

#ifndef INCLUDE_ADC_H
#define INCLUDE_ADC_H

#include <driver/gpio.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>

// https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32s3/api-reference/peripherals/adc.html
// TODO: there may be already a struct condensing these values
// consider calibrating using adc_vref_to_gpio()

typedef struct
{
    adc_unit_t unit;
    adc_channel_t channel;
    adc_atten_t atten;
    esp_adc_cal_characteristics_t *adc_chars;
} adc_t;


adc_t* adc_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten);
uint32_t adc_avg_read(adc_t* adc, int n);

#endif
