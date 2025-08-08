#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include "header.h"
#include "User_Setup.h"
#include <lvgl.h>

TFT_eSPI tft = TFT_eSPI();

// LVGL буфер кадру
static lv_disp_draw_buf_t lv_draw_buf;
static lv_color_t lv_buf1[222 * 40]; // ~40 ліній буфера

static void tft_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushPixels((uint16_t *)color_p, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  // I2C
  Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);

  // Підсвітка
  pinMode(BOARD_TFT_BL, OUTPUT);
  digitalWrite(BOARD_TFT_BL, HIGH);

  // Дисплей
  tft.init();
  tft.setRotation(0); // портрет
  tft.fillScreen(TFT_BLACK);

  // LVGL
  lv_init();

  // Якщо використовуєте LV_CONF_SKIP, переконайтесь, що LV_COLOR_DEPTH=16 у build_flags
  lv_disp_draw_buf_init(&lv_draw_buf, lv_buf1, NULL, sizeof(lv_buf1) / sizeof(lv_color_t));

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 222;
  disp_drv.ver_res = 480;
  disp_drv.flush_cb = tft_flush;
  disp_drv.draw_buf = &lv_draw_buf;
  lv_disp_t *disp = lv_disp_drv_register(&disp_drv);
  (void)disp;

  // Проста сцена
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "LVGL OK\n222x480\nRot 0");
  lv_obj_center(label);
}

void loop() {
  static uint32_t last = 0;
  uint32_t now = millis();
  lv_tick_inc(now - last);
  last = now;
  lv_timer_handler();
  delay(5);
}
