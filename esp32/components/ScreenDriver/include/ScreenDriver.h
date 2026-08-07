#ifndef UI_H
#define UI_H
/*! @file
 * Methods for initializing and starting the UI.
 */
#include <stdint.h>

typedef enum UI_RESULT {
  UI_SUCCESS = 0,
  UI_LVGL_DISP_INIT_ERROR = 1,
  UI_LVGL_LOCK_FAILED = 2,
  y
} UI_RESULT;

typedef struct ui_lcd_gpio_config {
  uint32_t gpio_mosi;
  uint32_t gpio_clk;
  uint32_t gpio_cs;
  uint32_t gpio_dc;
  uint32_t gpio_rst;
  uint32_t gpio_bl;

} ui_lcd_gpio_config;

UI_RESULT ui_initialize(ui_lcd_gpio_config config);

#endif // UI_H
