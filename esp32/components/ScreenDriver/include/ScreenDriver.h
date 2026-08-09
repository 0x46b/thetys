/*! @file
 * Methods for initializing and starting the LCD-screen.
 * Copyright (C) 2026 Sebastian Murschall <sebastian.murschall@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SCREEN_DRIVER_H
#define SCREEN_DRIVER_H

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

#endif // SCREEN_DRIVER_H
