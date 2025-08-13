#include "lv_helper.h"
#include "header.h"
#include "ui/ui.h"



static lv_disp_draw_buf_t lv_draw_buf;
static lv_color_t lv_buf1[222 * 40];
static lv_disp_t *disp;
static home_button_callback_t homeCb = NULL; 

void tft_flush(lv_disp_drv_t *disp, const lv_area_t *area,
    lv_color_t *color_p)
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );
    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();
    lv_disp_flush_ready( disp );
}

void lv_touch_homekey_set_cb(home_button_callback_t  cb)
{
    homeCb = cb;
}

void touchHomeKeyCallback(void *user_data)
{
    Serial.println("Home key pressed!");
    static uint32_t checkMs = 0;
    if (millis() > checkMs) {
        if (homeCb) {
            homeCb(NULL);
        }
    }
    checkMs = millis() + 200;
}

static void touch_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    static int16_t x[5], y[5];
    uint8_t touched = touch.getPoint(x, y, touch.getSupportTouchPoint());
    if (touched) {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = x[0];  
    data->point.y = y[0];
    } else {
    data->state = LV_INDEV_STATE_REL;
    }
}

void lv_display_init() {
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    lv_disp_draw_buf_init(&lv_draw_buf, lv_buf1, NULL, sizeof(lv_buf1) / sizeof(lv_color_t));
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.flush_cb = tft_flush;
    disp_drv.draw_buf = &lv_draw_buf;
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);
    (void)disp;
}

void lv_touch_init() {
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read;
    indev_drv.disp = disp; 
    lv_indev_drv_register(&indev_drv);
}