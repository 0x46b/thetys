/* Methods for initializing and starting the LCD-Screen.
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

#include "ScreenDriver.h"
#include "definitions.h"
#include "esp_lcd_ili9341_init_cmds_1.h"
#include "misc/lv_color.h"
#include <demos/lv_demos.h>
#include <driver/spi_master.h>
#include <esp_lcd_ili9341.h>
#include <esp_lcd_io_spi.h>
#include <esp_lcd_panel_ops.h>
#include <esp_log.h>
#include <esp_lvgl_port.h>
#include <lvgl.h>

static const char *TAG = "UI";

static lv_disp_t *disp_handle;

UI_RESULT ui_initialize(ui_lcd_gpio_config config) {
  ESP_LOGD(TAG, "Initialize SPI bus");
  const spi_bus_config_t bus_config = ILI9341_PANEL_BUS_SPI_CONFIG(
      config.gpio_clk, config.gpio_mosi,
      DISP_HEIGHT * LVGL_DRAW_BUF_LINES * sizeof(uint16_t));

  ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &bus_config, SPI_DMA_CH_AUTO));

  const esp_lcd_panel_io_spi_config_t io_config =
      ILI9341_PANEL_IO_SPI_CONFIG(config.gpio_cs, config.gpio_dc, NULL, NULL);

  ESP_LOGD(TAG, "Install ILI9341 panel driver");
  ili9341_vendor_config_t my_ili9341_vendor_cfg = {
      .init_cmds = ili9341_lcd_init_vendor,
      .init_cmds_size =
          sizeof(ili9341_lcd_init_vendor) / sizeof(ili9341_lcd_init_cmd_t),
  };
  const esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = config.gpio_rst,
      .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
      .bits_per_pixel = 16,
      .vendor_config = &my_ili9341_vendor_cfg,
  };
  ESP_LOGD(TAG, "Registering LCD-io-handle");
  esp_lcd_panel_io_handle_t io_handle = NULL;
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST,
                                           &io_config, &io_handle));

  ESP_LOGD(TAG, "Creating panel-handle");
  esp_lcd_panel_handle_t panel_handle = NULL;
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, false));
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

  ESP_LOGD(TAG, "Initializing lvgl port");
  const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
  ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

  ESP_LOGD(TAG, "Adding lvgl display");
  const lvgl_port_display_cfg_t disp_cfg = {
      .io_handle = io_handle,
      .panel_handle = panel_handle,
      .buffer_size = DISP_WIDTH * DISP_HEIGHT,
      .double_buffer = true,
      .hres = DISP_WIDTH,
      .vres = DISP_HEIGHT,
      .color_format = LV_COLOR_FORMAT_RGB565_SWAPPED,
      .monochrome = false,
      /* Rotation values must be same as used in esp_lcd for initial settings of
         the screen */
      .rotation =
          {
              .swap_xy = true,
              .mirror_x = false,
              .mirror_y = false,
          },
      .flags = {.buff_dma = false, .buff_spiram = true}};
  disp_handle = lvgl_port_add_disp(&disp_cfg);
  if (disp_handle == NULL) {
    ESP_LOGE(TAG, "LVGL display-registration failed");
    return UI_LVGL_DISP_INIT_ERROR;
  }
  ESP_LOGI(TAG, "Successful initialized ScreenDriver");
  return UI_SUCCESS;
}
