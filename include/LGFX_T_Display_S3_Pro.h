#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "header.h"

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Bus_SPI    _bus_instance;
  lgfx::Panel_ST7796 _panel_instance;
  lgfx::Light_PWM  _light_instance;
  lgfx::Touch_CST816S _touch_instance;

public:
  LGFX(void)
  {
    // Конфігурація SPI шини
    {
      auto cfg = _bus_instance.config();
      
      // SPI піни для T-Display-S3-Pro
      cfg.spi_host = SPI3_HOST;
      cfg.freq_write = 40000000;
      cfg.freq_read  = 20000000;
      cfg.spi_3wire  = false;
      cfg.use_lock   = false;
      cfg.dma_channel = 1;
      cfg.pin_sclk = BOARD_SPI_SCK;
      cfg.pin_mosi = BOARD_SPI_MOSI;
      cfg.pin_miso = BOARD_SPI_MISO;
      cfg.pin_dc   = BOARD_TFT_DC;
      
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    // Конфігурація панелі ST7796
    {
      auto cfg = _panel_instance.config();
      
      cfg.pin_cs           = 39;
      cfg.pin_rst          = 47;
      cfg.pin_busy         = -1;
      
      cfg.panel_width      = 222;
      cfg.panel_height     = 480;
      cfg.offset_x         = 48;
      cfg.offset_y         = 0;
      cfg.offset_rotation  = 0;
      cfg.rgb_order        = false;
      cfg.invert           = true;
      
      cfg.readable         = false;  
      _panel_instance.config(cfg);
    }

    // Конфігурація підсвітки
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = BOARD_TFT_BL;
      cfg.invert = false;
      cfg.freq   = 44100;
      cfg.pwm_channel = 7;
      
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    setPanel(&_panel_instance);
  }
}; 