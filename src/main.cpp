#include <Arduino.h>
#include <Wire.h>
#include "header.h"
#include <TFT_eSPI.h>
#include "../lib/lvgl_8/lvgl.h"
// #include "../lib/lvgl_9/lvgl.h"
#include "ui/ui.h"
#include "TouchDrvCSTXXX.hpp"
#include <HardwareSerial.h>
#include "console.h"
#include "lv_helper.h"

TFT_eSPI tft = TFT_eSPI();
TouchDrvCSTXXX touch;
HardwareSerial Uart(1);

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("Serial OK");

  Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
  Uart.begin(115200, SERIAL_8N1, 44, 43);

  tft.init();
  tft.setSwapBytes(true);
  tft.setRotation(0); 
  tft.fillScreen(TFT_BLACK);

touch.setPins(BOARD_TOUCH_RST, BOARD_SENSOR_IRQ);
bool hasTouch = touch.begin(Wire, CST226SE_SLAVE_ADDRESS, TOUCH_SDA, TOUCH_CLK);
if (!hasTouch) {
    Serial.println("Failed to find Capacitive Touch !");
} else {
    Serial.println("Find Capacitive Touch");
    touch.setMaxCoordinates(TFT_WIDTH, TFT_HEIGHT);
    touch.setHomeButtonCallback(touchHomeKeyCallback, NULL);
    touch.setSwapXY(false);
    touch.setMirrorXY(false, false);
}
    pinMode(BOARD_TFT_BL, OUTPUT);
    digitalWrite(BOARD_TFT_BL, HIGH);

    lv_init();

    lv_display_init();
    lv_touch_init();
    ui_init();
    lv_touch_homekey_set_cb(return_home_cd);
  
}

void loop() {

    static uint32_t last = millis();
    uint32_t now = millis();
    lv_tick_inc(now - last);
    last = now;

    lv_timer_handler();
    
    // if (ui_consolescreen) {
    //     Console_run();
    // }
    
    delay(1);
}