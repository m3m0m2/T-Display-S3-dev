#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

// Specific to LCD display T-Display-S3 Lilygo ESP32-S3

// LCD
#define LCD_H_RES               320
#define LCD_V_RES               170
#define LCD_PIXEL_CLOCK_HZ      6528000
#define LCD_BK_LIGHT_ON_LEVEL   1
#define LCD_BK_LIGHT_OFF_LEVEL (!LCD_BK_LIGHT_ON_LEVEL)
#define LCD_CMD_BITS            8
#define LCD_PARAM_BITS          8

// Other LCD/LVGL parameters
#define LVGL_LCD_BUF_SIZE       (LCD_H_RES * LCD_V_RES)
#define LVGL_TICK_PERIOD_MS      2

// LCD PINs
#define PIN_NUM_RST         GPIO_NUM_5
#define PIN_NUM_CS          GPIO_NUM_6
#define PIN_NUM_DC          GPIO_NUM_7
#define PIN_NUM_PCLK        GPIO_NUM_8
#define PIN_RD              GPIO_NUM_9
#define PIN_PWR             GPIO_NUM_15
#define PIN_NUM_BK_LIGHT    GPIO_NUM_38
#define PIN_NUM_DATA0       GPIO_NUM_39
#define PIN_NUM_DATA1       GPIO_NUM_40
#define PIN_NUM_DATA2       GPIO_NUM_41
#define PIN_NUM_DATA3       GPIO_NUM_42
#define PIN_NUM_DATA4       GPIO_NUM_45
#define PIN_NUM_DATA5       GPIO_NUM_46
#define PIN_NUM_DATA6       GPIO_NUM_47
#define PIN_NUM_DATA7       GPIO_NUM_48

// Other PINs
#define PIN_BUTTON_1        GPIO_NUM_0
#define PIN_BAT_VOLT        GPIO_NUM_4
#define PIN_BUTTON_2        GPIO_NUM_14

// External buttons and external connections
#define PIN_BUTTON_3        GPIO_NUM_2

#define GPIO_INPUT_PINS_MASK  ((1ULL<<PIN_BUTTON_1) | \
            (1ULL<<PIN_BUTTON_2) | (1ULL<<PIN_BUTTON_3))

// Touch screen missing on my model
//#define PIN_I2C_SCL                     17
//#define PIN_I2C_SDA                     18
//#define PIN_TOUCH_INT                   16
//#define PIN_TOUCH_RES                   21

#endif
