#include <Arduino.h>
#include <Wire.h>
#include "header.h"
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "ui/ui.h"
#include "TouchDrvCSTXXX.hpp"
#include <HardwareSerial.h>

#define UART_BUF_SIZE 1024
#define DISPLAY_BUF_SIZE 1024  // розмір буфера під весь лог
#define MAX_LINE_LENGTH 100    // максимум символів у рядку
#define LINE_HISTORY 80  

// Оголошення UI об'єктів
extern lv_obj_t *ui_textarea1;

TFT_eSPI tft = TFT_eSPI();
TouchDrvCSTXXX touch;
HardwareSerial Uart(1);
static lv_disp_draw_buf_t lv_draw_buf;
static lv_color_t lv_buf1[222 * 40];



char uart_buffer[UART_BUF_SIZE];
char display_buffer[UART_BUF_SIZE];
static uint16_t display_buffer_len = 0;
volatile uint16_t write_pos = 0;
volatile uint16_t read_pos = 0;

void uart_receive_process();
void update_ui_text(); 

static void tft_flush(lv_disp_drv_t *disp, const lv_area_t *area,
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

static void btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_child(btn, 0); 
    if(code == LV_EVENT_PRESSED) {
      lv_obj_set_style_text_color(label, lv_color_hex(0xFF0000), 0); 
      lv_obj_set_style_bg_color(btn, lv_color_hex(0xFF0000), LV_STATE_PRESSED);
    } else if(code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST) {
      lv_obj_set_style_text_color(label, lv_color_white(), 0); 
      lv_obj_set_style_bg_color(btn, lv_color_hex(0x87CEEB), 0);
    }
  }

void touchHomeKeyCallback(void *user_data)
{
    Serial.println("Home key pressed!");
    static uint32_t checkMs = 0;
    if (millis() > checkMs) {
        _ui_screen_change( &ui_mainscreen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, &ui_mainscreen_screen_init);
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
    lv_label_set_text_fmt(ui_value, "X: %d, Y: %d", data->point.x, data->point.y);
}

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
 
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  lv_disp_draw_buf_init(&lv_draw_buf, lv_buf1, NULL, sizeof(lv_buf1) / sizeof(lv_color_t));
  disp_drv.hor_res = TFT_WIDTH;
  disp_drv.ver_res = TFT_HEIGHT;
  disp_drv.flush_cb = tft_flush;
  disp_drv.draw_buf = &lv_draw_buf;
  lv_disp_t *disp = lv_disp_drv_register(&disp_drv);
  (void)disp;

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touch_read;
  indev_drv.disp = disp; 
  lv_indev_drv_register(&indev_drv);

  ui_init();

  // Ініціалізація UART буфера
  uart_buffer[0] = '\0';

}

void loop() {

    static uint32_t last = millis();
    uint32_t now = millis();
    lv_tick_inc(now - last);
    last = now;

    lv_timer_handler();
    
    
    static uint32_t last_update = 0;
    if (millis() - last_update > 10) {
        uart_receive_process();
        update_ui_text();
        last_update = millis();
    }

    
    // update_ui_text();
    delay(1);
}

void add_line_to_display(const char *line) {
    size_t line_len = strlen(line);
    if (line_len > MAX_LINE_LENGTH) line_len = MAX_LINE_LENGTH; // обмеження довжини рядка

    // Перевірка чи вміститься новий рядок + '\n' + '\0'
    while (display_buffer_len + line_len + 2 >= DISPLAY_BUF_SIZE) {
        // Видаляємо найстарший рядок
        char *first_newline = strchr(display_buffer, '\n');
        if (first_newline) {
            size_t shift_len = first_newline - display_buffer + 1;
            memmove(display_buffer, display_buffer + shift_len, display_buffer_len - shift_len);
            display_buffer_len -= shift_len;
        } else {
            display_buffer_len = 0;
            display_buffer[0] = '\0';
            break;
        }
    }

    // Додаємо новий рядок в кінець
    memcpy(display_buffer + display_buffer_len, line, line_len);
    display_buffer_len += line_len;
    display_buffer[display_buffer_len++] = '\n';
    display_buffer[display_buffer_len] = '\0';
}

void uart_receive_process() {
    while (Uart.available() > 0) {
        char c = Uart.read();
        
        uint16_t next_write_pos = (write_pos + 1) % UART_BUF_SIZE;
        
        // Перевірка переповнення
        if (next_write_pos != read_pos) {
            // Приймаємо тільки валідні символи
            if ((c >= 32 && c <= 126) || c == '\n' || c == '\t' || c == '\r' || c == '\0') {
                uart_buffer[write_pos] = c;
                write_pos = next_write_pos;
                
                // Debug інформація (можна видалити)
                Serial.print("Write pos: ");
                Serial.println(write_pos);
                Serial.print("Char: ");
                Serial.println(c);
            }
        } else {
            Serial.println("UART buffer overflow");
        }
    }
}

void update_ui_text() {
    static char string_to_send[100];
    static uint8_t string_index = 0;
    static uint32_t last_ui_update = 0;

    while (read_pos != write_pos) {
        char c = uart_buffer[read_pos];
        read_pos = (read_pos + 1) % UART_BUF_SIZE;

        if (c >= 32 && c <= 126) {  // звичайні символи
            if (string_index < sizeof(string_to_send) - 1) {
                string_to_send[string_index++] = c;
            }
        } 
        else if (c == '\n' || c == '\r') {  // кінець рядка
            if (string_index > 0) {
                string_to_send[string_index] = '\0';
                add_line_to_display(string_to_send);
                Serial.println(string_to_send);
                string_index = 0;
            }
        }
    }

    if (millis() - last_ui_update > 10) {
        lv_textarea_set_text(ui_textarea1, display_buffer);
        lv_textarea_set_cursor_pos(ui_textarea1, LV_TEXTAREA_CURSOR_LAST); // автопрокрутка вниз
        last_ui_update = millis();
    }
}


// void SwitchApp(App new_app) {
//     if (current_app != new_app) {
//         Serial.printf("Switching to app: %d\n", new_app);
//         switch (new_app) {
//             case HOME:
//                 ui_consolescreen_screen_destroy();
//                 ui_mainscreen_screen_init();
//                 break;
//             case CONSOLE:
//                 ui_mainscreen_screen_destroy();
//                 ui_consolescreen_screen_init();
//                 break;
//         }
//         current_app = new_app;
//     }
// }