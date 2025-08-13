#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ui.h>
#include "touch.h"
#include "utilities.h"
#include "pmu.h" // Include the PMU header

// https://www.youtube.com/nishad2m8
// https://buymeacoffee.com/nishad2m8

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 222;
static const uint16_t screenHeight = 480;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 10 ];

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

// Display flushing function
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

// Touchpad reading function
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    if (touch_touched()) {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touch_last_x;
        data->point.y = touch_last_y;

       // Serial.print("Data x ");
       // Serial.println(touch_last_x);

       // Serial.print("Data y ");
       // Serial.println(touch_last_y);
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

void setup() {
    Serial.begin(115200);

    lv_init();

    #if LV_USE_LOG != 0
        lv_log_register_print_cb(my_print);
    #endif

    tft.begin();

    // Set the orientation of the TFT display
    // The setRotation function adjusts the orientation of the display according to the parameter passed to it.
    // Parameter values and their corresponding orientations:
    //   0: Home button down (portrait mode, default configuration) - 222x480 resolution
    //   1: Rotate clockwise to landscape mode - 480x222 resolution
    //   2: Home button up (upside-down portrait mode)
    //   3: Rotate counterclockwise to landscape mode
    tft.setRotation(0); // Set the display to default portrait mode with home button down

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    touch_init(screenWidth, screenHeight, tft.getRotation());

    ui_init();
    Serial.println("Setup done");

    // Initialize PMU
    if (!initPMU()) {
        Serial.println("PMU initialization failed");
    }
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay(5);

    // Update and print PMU status
    updatePMU();  // Call the PMU update function to print voltage, percentage, and charging status
    delay(1000);  // Delay for 1 second before checking again
}
