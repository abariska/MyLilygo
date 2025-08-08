/**
 * @file      utilities.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-09-25
 *
 */
#pragma once
// T-Display-Pro pinmap

#define USING_DISPLAY_PRO_V1
#define DEFAULT_CPU_FREQ_240MHZ
#define HAS_PSRAM

// BUTTON Pinmap
// #define BOARD_USER_BUTTON   {0 /*btn1*/,12/*btn2*/,16/*btn3*/}
#define BOARD_BUTTON_L 0
#define BOARD_BUTTON_R_TOP 12
#define BOARD_BUTTON_R_BOTTOM 16
#define BOARD_BUTTON_NUM  3

// LED Pinmap
#define BOARD_LED 38

// Display Pinmap
#define BOARD_SPI_MISO      8
#define BOARD_SPI_MOSI      17
#define BOARD_SPI_SCK       18
#define BOARD_TFT_CS        39
#define BOARD_TFT_RST       47
#define BOARD_TFT_DC        9
#define BOARD_TFT_BL        48


// I2C Bus
#define BOARD_I2C_SDA       5 // SDA touch, sensor, external i2C
#define BOARD_I2C_SCL       6 // SCL touch, sensor, external i2C

// Touch Sensor CST226SE address
#define BOARD_TOUCH_ADDR    0x5A
#define BOARD_TOUCH_RST     13
#define BOARD_TOUCH_IRQ     21

// Sensor LTR-553ALS-01 address
#define BOARD_SENSOR_LTR_ADDR 0x23




// SD , TFT share SPI Bus
#define BOARD_SPI_MISO      8
#define BOARD_SPI_MOSI      17
#define BOARD_SPI_SCK       18
#define BOARD_TFT_CS        39
#define BOARD_TFT_RST       47
#define BOARD_TFT_DC        9
#define BOARD_TFT_BL        48
#define BOARD_SD_CS         14
#define BOARD_SENSOR_IRQ    21
#define BOARD_TOUCH_RST     13

#define BOARD_TFT_WIDTH     222
#define BOARD_TFT_HEIHT     480



// Camera Shield pinmap
#define CAMERA_PIN_RESET    (-1)
#define CAMERA_PIN_SIOD     BOARD_I2C_SDA
#define CAMERA_PIN_SIOC     BOARD_I2C_SCL
#define CAMERA_PIN_VSYNC    (7)
#define CAMERA_PIN_PWDN     (46)
#define CAMERA_PIN_HREF     (15)
#define CAMERA_PIN_Y9       (4)
#define CAMERA_PIN_XCLK     (11)
#define CAMERA_PIN_Y8       (10)
#define CAMERA_PIN_Y7       (3)
#define CAMERA_PIN_Y6       (1)
#define CAMERA_PIN_Y5       (42)
#define CAMERA_PIN_Y4       (40)
#define CAMERA_PIN_Y3       (41)
#define CAMERA_PIN_Y2       (45)
#define CAMERA_PIN_PCLK     (2)

#define XCLK_FREQ_HZ        20000000


#define CAMERA_LEC_CH               0
#define LEDC_TFT_CH                 1
#define LEDC_WHITE_CH               2

// #define LEDC_IR_CH          1
// #define CAMERA_IR_LED       (46)


#ifdef USING_DISPLAY_PRO_V1
#define BRIGHTNESS_MAX_LEVEL        255
#else
#define BRIGHTNESS_MAX_LEVEL        16
#endif
