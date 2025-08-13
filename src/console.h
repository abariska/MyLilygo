#include "ui/ui.h"
#include <HardwareSerial.h>

#pragma once

#define UART_BUF_SIZE 1024
#define DISPLAY_BUF_SIZE 1024  // розмір буфера під весь лог
#define MAX_LINE_LENGTH 100    // максимум символів у рядку
#define LINE_HISTORY 30  

extern lv_obj_t * log_list;
extern lv_obj_t * log_items[LINE_HISTORY];
extern uint16_t log_count;

extern HardwareSerial Uart;

void log_add_line(const char * text);
void uart_receive_process();
void Console_run();