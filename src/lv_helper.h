#ifndef LV_HELPER_H
#define LV_HELPER_H

#pragma once

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <TouchDrvCSTXXX.hpp>

extern TFT_eSPI tft;
extern TouchDrvCSTXXX touch;

void tft_flush(lv_disp_drv_t *disp, const lv_area_t *area,
    lv_color_t *color_p);

void touchHomeKeyCallback(void *user_data);

void lv_display_init();

void lv_touch_init();

void lv_touch_homekey_set_cb(home_button_callback_t  cb);





#endif