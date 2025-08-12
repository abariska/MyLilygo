#ifndef LV_CONF_H
#define LV_CONF_H

/* Make sure attributes are available when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Enable LGVG logging */
#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN   /* Use WARN by default; change to USER or DEBUG during dev */
#define LV_LOG_PRINTF 1
#define LV_LOG_TRACE_MEM 1
#define LV_TEXTAREA_DEF_MAX_LEN 1024


/* Horizontal and vertical resolution */
#define LV_HOR_RES_MAX (222)
#define LV_VER_RES_MAX (480)

/* Color depth */
#define LV_COLOR_DEPTH 16
#define LV_COLOR_SCREEN_TRANSP 0

/* Default display orientation */
#define LV_DISP_DEF_REFR_PERIOD 30 // 30 ms, ~33 FPS

/* GPU features (not used on ESP32) */
#define LV_USE_GPU_NONE 1

/* Memory allocator setting */
#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE (1024U * 20)       /* 20 kB of LVGL memory pool; increase if complex screens + fonts */

/* Enable support for printing formatted text to labels */
#define LV_LABEL_TEXT_SELECTION 0
#define LV_USE_LABEL 1
#define LV_USE_LABEL_TEXT_FMT 1
#define LV_FONT_MONTSERRAT_16 1

/* Basic modules */
#define LV_USE_BTN 1
#define LV_USE_IMG 1
#define LV_USE_SLIDER 1
#define LV_USE_BAR 1
#define LV_USE_SWITCH 1

/* Input device (touch) */
#define LV_USE_INDEV 1
#define LV_INDEV_DEF_READ_PERIOD 30  /* 30 ms */

/* Animations */
#define LV_USE_ANIMATION 1

/* Themes */
#define LV_USE_THEME_DEFAULT 1

/* File system */
#define LV_USE_FS_STDIO 0

/* Enable examples to test modules (remove for production) */
#define LV_USE_PERF_MONITOR 0
#define LV_USE_DEMO_WIDGETS 0

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_CONF_H */
