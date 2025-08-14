#include "console.h"
#include <Arduino.h>
#include <HardwareSerial.h>

extern HardwareSerial Uart;

uint16_t log_count = 0;
lv_obj_t * log_items[LINE_HISTORY];


void log_add_line(const char * text) {
    // Якщо кількість елементів досягла максимуму — видаляємо перший (найстаріший)
    if (log_count >= LINE_HISTORY) {
        lv_obj_del(log_items[0]);

        // Зсув масиву вліво
        for (uint16_t i = 1; i < LINE_HISTORY; i++) {
            log_items[i - 1] = log_items[i];
        }
        log_count--;
    }

    // Додаємо новий рядок у кінець
    lv_obj_t * btn = lv_list_add_text(log_list, text);
    log_items[log_count++] = btn;

    // Автопрокрутка вниз
    lv_obj_scroll_to_view(btn, LV_ANIM_OFF);
}

void uart_receive_process() {
    static char line_buf[MAX_LINE_LENGTH];
    static uint8_t idx = 0;

    while (Uart.available() > 0) {
        char c = Uart.read();
        if ((c >= 32 && c <= 126) || c == '\t') {
            if (idx < MAX_LINE_LENGTH - 1) {
                line_buf[idx++] = c;
            }
        } else if (c == '\n' || c == '\r') {
            if (idx > 0) {
                line_buf[idx] = '\0';
                log_add_line(line_buf); // Додаємо готовий рядок у лог
                idx = 0;
            }
        }
    }
}

void Console_run() {
    static uint32_t last_update = 0;
    if (millis() - last_update > 10) {
        uart_receive_process();
        last_update = millis();
        // Serial.println("Console_run");
    }
}


// void ui_consolescreen_screen_init(void)
// {
// ui_consolescreen = lv_obj_create(NULL);
// lv_obj_clear_flag( ui_consolescreen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
// lv_obj_set_style_bg_color(ui_consolescreen, lv_color_hex(0x001F1C), LV_PART_MAIN | LV_STATE_DEFAULT );
// lv_obj_set_style_bg_opa(ui_consolescreen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

// log_list = lv_list_create(ui_consolescreen);
// lv_obj_set_size( log_list, lv_pct(100), lv_pct(100));
// lv_obj_align(log_list, LV_ALIGN_TOP_LEFT, 0, 0);
// lv_obj_set_scrollbar_mode(log_list, LV_SCROLLBAR_MODE_AUTO);
// lv_obj_set_scroll_dir(log_list, LV_DIR_VER);
// lv_obj_set_scroll_snap_x(log_list, LV_SCROLL_SNAP_END);
// lv_obj_set_style_text_color(log_list, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
// lv_obj_set_style_text_opa(log_list, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
// lv_obj_set_style_bg_color(log_list, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
// lv_obj_set_style_bg_opa(log_list, 255, LV_PART_MAIN| LV_STATE_DEFAULT);


// lv_obj_set_style_pad_all(log_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
// }