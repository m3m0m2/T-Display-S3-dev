#include "adc.h"

#include <stdlib.h>

static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static int DEFAULT_VREF = 1100;

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
          printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
          printf("Characterized using eFuse Vref\n");
    } else {
          printf("Characterized using Default Vref\n");
    }
}

// TODO: for more accuracy consider using efuse, however ESP32-S3 does not seem to have
// calibration data burned in, but may be possible to write it
// example: esp-idf/examples/peripherals/adc/single_read/adc/

adc_t* adc_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten)
{
    adc_t* adc = (adc_t*)malloc(sizeof(adc_t));

    adc->unit = unit;
    adc->channel = channel;
    adc->atten = atten;

    //Configure ADC
    if (unit == ADC_UNIT_1)
    {
        adc1_config_width(width);
        adc1_config_channel_atten((adc1_channel_t)channel, atten);
    }
    else
    {
        adc2_config_channel_atten((adc2_channel_t)channel, atten);
    }

    //Characterize ADC
    adc->adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc->adc_chars);
    print_char_val_type(val_type);

    return adc;
}

uint32_t adc_avg_read(adc_t* adc, int n)
{
    uint32_t adc_reading = 0;

    for (int i=0; i<n; i++)
    {
        if (adc->unit == ADC_UNIT_1)
            adc_reading += adc1_get_raw((adc1_channel_t)adc->channel);
        else
        {
            int raw;
            adc2_get_raw((adc2_channel_t)adc->channel, width, &raw);
            adc_reading += raw;
        }
    }
    adc_reading /= n;

    //Convert adc_reading to voltage in mV
    uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc->adc_chars);
    return voltage;
}
