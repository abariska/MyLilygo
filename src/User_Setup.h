// T-Display-S3-Pro + LVGL конфігурація
#define USER_SETUP_ID 214

#define ST7796_DRIVER
#define TFT_WIDTH  222
#define TFT_HEIGHT 480
#define CGRAM_OFFSET 48
#define TFT_RGB_ORDER TFT_BGR 
#define TFT_INVERSION_ON 0

// ⭐ DMA для LVGL
#define ESP32_DMA

// Піни
#define TFT_MISO  8
#define TFT_MOSI  17
#define TFT_SCLK  18
#define TFT_CS    39
#define TFT_DC    9
#define TFT_RST   47
#define TFT_BL    48
#define TFT_BACKLIGHT_ON    HIGH

// Тач налаштування
#define TOUCH_CS 5
#define TOUCH_IRQ 21
#define TOUCH_CLK 6
#define TOUCH_RST 13

// Швидкість
#define SPI_FREQUENCY      40000000
#define SPI_READ_FREQUENCY 20000000

// Шрифти для LVGL
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_GFXFF
#define SMOOTH_FONT 