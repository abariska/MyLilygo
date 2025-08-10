// User_Setup.h for T-Display-S3-Pro (ESP32-S3)
// This file is used by TFT_eSPI when USER_SETUP_LOADED is defined

#ifndef USER_SETUP_H_CUSTOM
#define USER_SETUP_H_CUSTOM

#define USER_SETUP_ID 214

// Panel
#define ST7796_DRIVER
#define TFT_WIDTH   222
#define TFT_HEIGHT  480
#define CGRAM_OFFSET 48
#define TFT_RGB_ORDER TFT_BGR
#define TFT_INVERSION_ON 1
#define TOUCH_CS -1

// Pins (match include/header.h)
#define TFT_MISO 8
#define TFT_MOSI 17
#define TFT_SCLK 18
#define TFT_CS   39
#define TFT_DC   9
#define TFT_RST  47
#define TFT_BL   48
#define TFT_BACKLIGHT_ON HIGH

// Touch
#define TOUCH_SDA 5
#define TOUCH_IRQ 21
#define TOUCH_CLK 6
#define TOUCH_RST 13

// SPI speeds
#define SPI_FREQUENCY      40000000
#define SPI_READ_FREQUENCY 20000000

// Fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_GFXFF
#define SMOOTH_FONT

// Touch is handled by external driver, disable TFT_eSPI touch
//#define TOUCH_CS -1

#endif // USER_SETUP_H_CUSTOM 