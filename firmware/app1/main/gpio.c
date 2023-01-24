/*
 * Copyright (C) 2023, Mauro Meneghin
 */

#include "config.h"
#include "gpio.h"

#include <stdio.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

static const char* TAG = "GPIO";
static xQueueHandle gpio_evt_queue = NULL;
static input_handler_t* input_handler = NULL;
static gpio_in_t gpio_in;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t pin = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &pin, NULL);
}

static void gpio_input_loop(void* arg)
{
    uint32_t pin;

    while (1)
    {
        if(xQueueReceive(gpio_evt_queue, &pin, portMAX_DELAY))
        {
            int value = gpio_get_level(pin);
            ESP_LOGI(TAG, "GPIO[%u] intr, val: %d\n", pin, value);

            gpio_set_pin(&gpio_in, pin, value);

            if (input_handler)
                input_handler(pin, value);
        }
    }
}

void gpio_register_handler(input_handler_t* handler)
{
    input_handler = handler;
}

gpio_in_t gpio_get_state()
{
    return gpio_in;
}

int gpio_get_pin(gpio_in_t* state, int pin)
{
    if (state == NULL)
        state = &gpio_in;

    uint32_t* word = state->data;
    if (pin >= 32)
    {
        word++;
        pin -= 32;
    }

    return (*word & (1 << pin)) != 0;
}

void gpio_set_pin(gpio_in_t* state, int pin, int value)
{
    if (state == NULL)
        state = &gpio_in;

    uint32_t* word = state->data;
    if (pin >= 32)
    {
        word++;
        pin -= 32;
    }

    if (value)
        *word |= (1 << pin);
    else
        *word &= ~(1 << pin);
}

#define ESP_INTR_FLAG_DEFAULT 0

static void init_pin(uint32_t pin)
{
    //hook isr handler for specific input GPIO PIN
    gpio_isr_handler_add(pin, gpio_isr_handler, (void*) pin);
    gpio_set_pin(&gpio_in, pin, gpio_get_level(pin));
}

void gpio_init()
{
    // zero gpio_in
    gpio_in.data[0] = 0;
    gpio_in.data[1] = 0;

    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //interrupt on edge rising/falling edge
    io_conf.intr_type = GPIO_INTR_POSEDGE /* raising edge */ |
                        GPIO_INTR_NEGEDGE /* falling edge */;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PINS_MASK;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_input_loop, "gpio_input_loop", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    init_pin(PIN_BUTTON_1);
    init_pin(PIN_BUTTON_2);

    // external button
    init_pin(PIN_BUTTON_3);
}
