#include "BluetoothManager.h"
#include "RGBLEDDriver.h"
#include "RelaisDriver.h"
#include "ScreenDriver.h"
#include "SensorDriver.h"
#include "SensorManager.h"
#include "thetys_ui.h"
#include "vars.h"
#include <demos/lv_demos.h>
#include <driver/spi_master.h>
#include <esp_lcd_ili9341.h>
#include <esp_lcd_io_spi.h>
#include <esp_lcd_panel_ops.h>
#include <esp_log.h>
#include <esp_lvgl_port.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <lvgl.h>
#include <sdkconfig.h>

#define SENSOR_GPIO 1
#define PUMP_GPIO 40

#define LCD_HOST SPI2_HOST
#define LCD_DIN 11
#define LCD_CLK 12
#define LCD_CS 10
#define LCD_DC 9
#define LCD_RST 46
#define LCD_BL 13

static const char *TAG = "app_main";

void pump_task(void *params) {
  led_drv_set_color(0, 50, 0);
  ESP_LOGI(TAG, "Starting pump for 30 seconds");
  relais_drv_open();
  vTaskDelay(pdMS_TO_TICKS(10000));
  ESP_LOGI(TAG, "Stopping pump");
  relais_drv_close();
  led_drv_set_color(50, 0, 0);

  vTaskDelete(NULL);
}

void start_ui(void) {
  ui_lcd_gpio_config config = {.gpio_mosi = LCD_DIN,
                               .gpio_clk = LCD_CLK,
                               .gpio_cs = LCD_CS,
                               .gpio_dc = LCD_DC,
                               .gpio_rst = LCD_RST,
                               .gpio_bl = LCD_BL};
  UI_RESULT ui_result = ui_initialize(config);

  if (ui_result != UI_SUCCESS) {
    ESP_LOGE(TAG, "Display initialization failed. Could not start UI.");
    abort();
  }

  ui_result = start_thetys_ui();
  if (ui_result != UI_SUCCESS) {
    ESP_LOGE(TAG, "Could not start UI.");
    abort();
  }
}

void update_sensor_data(uint32_t sensorId, uint32_t value) {
  // if (lvgl_port_lock(0)) {
  ESP_LOGI(TAG, "New sample for sensor %i: %i", sensorId, value);
  if (sensorId == 0) {
    set_var__humidity_level(value);

    if (value < 60) {
      set_var__pump_running(true);
      led_drv_set_to(BLUE);
    } else {
      set_var__pump_running(false);
      led_drv_set_to(GREEN);
    }
  }
  /* } else { */
  /*   ESP_LOGE("UI", "LVGL-Lock could not get acquired!"); */
  /*   return; */
  /*}*/
}

void app_main(void) {
  led_drv_initialize();
  led_drv_set_brightness(50);
  led_drv_set_to(CYAN);
  // blu_initialize();

  SENSOR_RESULT sensor_result = sensor_mgr_initialize(CONFIG_MAX_SENSOR_COUNT);
  if (sensor_result != SENSOR_SUCCESS) {
    ESP_LOGE(TAG, "Failed to initialize sensor-driver. Aborting");
    abort();
  }
  sensor_mgr_add_sensor(SENSOR_GPIO);
  sensor_mgr_subscribe_new_measurement(update_sensor_data);
  // relais_drv_initialize(PUMP_GPIO);
  /* xTaskCreate(pump_task,   // Function that implements the task */
  /*             "Pump-Task", // Text name for the task (for debugging) */
  /*             2048,        // Stack size in bytes (not words, in ESP-IDF) */
  /*             NULL,        // Parameter passed into the task */
  /*             5,           // Task priority (higher number = higher priority)
   */
  /*             NULL // Task handle (use if you want to delete/suspend it
   * later) */
  /* ); */

  start_sensor_polling_task();
  start_ui();
  led_drv_set_to(GREEN);
}
