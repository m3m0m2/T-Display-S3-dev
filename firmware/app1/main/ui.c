/*
 * Copyright (C) 2023, Mauro Meneghin
 */

#include "config.h"
#include "gpio.h"
#include "ui.h"

#include <esp_log.h>
#include <math.h>

static const char* TAG = "UI";

static lv_color_t colors[] =
{
    LV_COLOR_MAKE(232, 87, 116),        // Red
    LV_COLOR_MAKE(0x31, 0x5b, 0xa1),    // Azure
    LV_COLOR_MAKE(0, 0, 0)              // Black
};

static lv_style_t styles[2];

typedef struct
{
    lv_obj_t *scr;
    int count_val;
} my_timer_context_t;

typedef struct
{
    lv_obj_t* author;
    lv_obj_t* built;
} page1_t;

typedef struct
{
    int din[3];
    lv_obj_t* labels[4];
} page2_t;

static page1_t page1;
static page2_t page2;
static ui_data_t* ui_data;

static void init_page2_din()
{
    page2.din[0] = gpio_get_pin(NULL, 0);
    page2.din[1] = gpio_get_pin(NULL, 2);
    page2.din[2] = gpio_get_pin(NULL, 14);
}

static int get_adc0()
{
    return adc_avg_read(ui_data->adc0, 4);
}

static void show_adc_read()
{
    lv_label_set_text_fmt(
        page2.labels[3],
        "%u", get_adc0());
}

static void init_page2(lv_obj_t *scr)
{
    static lv_coord_t col_dsc[] = {118, 88, 28, 78, LV_GRID_TEMPLATE_LAST};  // 79*4 < 320
    static lv_coord_t row_dsc[] = {31, 31, 31, 31, 31, LV_GRID_TEMPLATE_LAST}; //31*5 < 170

    // Create a container with grid
    lv_obj_t* cont = lv_obj_create(scr); //lv_scr_act());
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);

    // Remove internal padding between cells
    lv_obj_set_style_pad_row(cont, 0, 0);
    lv_obj_set_style_pad_column(cont, 0, 0);
    // Remove frame margin
    lv_obj_set_style_pad_all(cont, LV_PART_MAIN, 0);

    // set size to container
    lv_obj_set_size(cont, lv_obj_get_width(lv_scr_act()), lv_obj_get_height(lv_scr_act()));
    //lv_obj_set_size(cont, LCD_H_RES, LCD_V_RES);

    lv_obj_center(cont);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);

    lv_obj_t* label;
    lv_obj_t* obj;
    uint8_t row, col;

    init_page2_din();

    {
        row=0; col=0;
        obj = lv_obj_create(cont);
        lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
                             LV_GRID_ALIGN_STRETCH, row, 1);

        label = lv_label_create(obj);
        lv_obj_add_style(label, &styles[0], 0);
        lv_label_set_text_fmt(label, "IN0");
        lv_obj_center(label);

        col += 1;
        obj = lv_obj_create(cont);
        lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
                             LV_GRID_ALIGN_STRETCH, row, 1);

        label = lv_label_create(obj);
        lv_obj_add_style(label, &styles[1], 0);
        lv_label_set_text_fmt(label, "%u", page2.din[0]);
        lv_obj_center(label);
        page2.labels[0] = label;
    }
    {
        row=1; col=0;
        obj = lv_obj_create(cont);
        lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
                             LV_GRID_ALIGN_STRETCH, row, 1);

        label = lv_label_create(obj);
        lv_obj_add_style(label, &styles[0], 0);
        lv_label_set_text_fmt(label, "IN2");
        lv_obj_center(label);

        col += 1;
        obj = lv_obj_create(cont);
        lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
                             LV_GRID_ALIGN_STRETCH, row, 1);

        label = lv_label_create(obj);
        lv_obj_add_style(label, &styles[1], 0);
        lv_label_set_text_fmt(label, "%u", page2.din[1]);
        lv_obj_center(label);
        page2.labels[1] = label;
    }
    {
        row=2; col=0;
        obj = lv_obj_create(cont);
        lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
                             LV_GRID_ALIGN_STRETCH, row, 1);

        label = lv_label_create(obj);
        lv_obj_add_style(label, &styles[0], 0);
        lv_label_set_text_fmt(label, "IN14");
        lv_obj_center(label);

        col += 1;
        obj = lv_obj_create(cont);
        lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
                             LV_GRID_ALIGN_STRETCH, row, 1);

        label = lv_label_create(obj);
        lv_obj_add_style(label, &styles[1], 0);
        lv_label_set_text_fmt(label, "%u", page2.din[2]);
        lv_obj_center(label);
        page2.labels[2] = label;
    }
    {
        row=3; col=0;
        obj = lv_obj_create(cont);
        lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
                             LV_GRID_ALIGN_STRETCH, row, 1);

        label = lv_label_create(obj);
        lv_obj_add_style(label, &styles[0], 0);
        lv_label_set_text_fmt(label, "ADC0");
        lv_obj_center(label);

        col += 1;
        obj = lv_obj_create(cont);
        lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
                             LV_GRID_ALIGN_STRETCH, row, 1);

        label = lv_label_create(obj);
        lv_obj_add_style(label, &styles[1], 0);
        lv_label_set_text_fmt(label, "%u", get_adc0());
        lv_obj_center(label);
        page2.labels[3] = label;
    }

    lv_timer_create(show_adc_read, 100, NULL);
}

static void start_anim_timer_cb(lv_timer_t *timer)
{
    my_timer_context_t *timer_ctx = (my_timer_context_t *) timer->user_data;
    int count = timer_ctx->count_val;

    //ESP_LOGI(TAG, "anim_timer_cb %d", count);

    if (count >= 10 && count < 80) {
        lv_coord_t offset = count;
        lv_obj_align(page1.author, LV_ALIGN_TOP_MID, 0, 10+offset);
        lv_obj_align(page1.built, LV_ALIGN_TOP_MID, 0, 40+offset);
    }

    count += 1;
    timer_ctx->count_val = count;

    // Delete timer when all animation finished
    if (count == 100) {

        // clear all screen after a whiel:
        lv_obj_clean(lv_scr_act());

        // remove anim timer
        lv_timer_del(timer);

        init_page2(timer_ctx->scr);
    }
}

void ui_create(ui_data_t* ui_data_)
{
    ui_data = ui_data_;
    // Create styles
    lv_style_init(&styles[0]);
    lv_style_set_text_font(&styles[0], &lv_font_montserrat_28);
    lv_style_set_text_color(&styles[0], colors[0]);

    lv_style_init(&styles[1]);
    lv_style_set_text_font(&styles[1], &lv_font_montserrat_28);
    lv_style_set_text_color(&styles[1], colors[1]);

    // Create labels
    lv_obj_t* label = lv_label_create(ui_data->scr);
    lv_obj_add_style(label, &styles[0], 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);
    lv_label_set_text(label, "(c) Mauro Meneghin");
    page1.author = label;

    label = lv_label_create(ui_data->scr);
    lv_obj_add_style(label, &styles[1], 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 50);
    lv_label_set_text(label, __DATE__ " " __TIME__);
    page1.built = label;

    // Create timer for animation
    static my_timer_context_t my_tim_ctx = {
        .count_val = 0,
    };
    my_tim_ctx.scr = ui_data->scr;
    //lv_timer_t * timer =
    lv_timer_create(start_anim_timer_cb, 80, &my_tim_ctx);
}

void ui_input_handler(uint32_t pin, int value)
{
    int index;

    if (pin == 0)
        index = 0;
    else if (pin == 2)
        index = 1;
    else if (pin == 14)
        index = 2;
    else
    {
        ESP_LOGE(TAG, "Unexpected PIN %d", pin);
        return;
    }

    page2.din[index] = value;

    lv_label_set_text_fmt(page2.labels[index], "%d", value);
}
