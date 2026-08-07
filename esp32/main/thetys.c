#include "BluetoothManager.h"
#include "RGBLEDDriver.h"
#include "RelaisDriver.h"
#include "SensorDriver.h"
#include "SensorManager.h"
#include "UI.h"
#include "esp_log_level.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <demos/lv_demos.h>
#include <driver/spi_master.h>
#include <esp_lcd_ili9341.h>
#include <esp_lcd_io_spi.h>
#include <esp_lcd_panel_ops.h>
#include <esp_log.h>
#include <esp_lvgl_port.h>
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

void app_main(void) {
  led_drv_initialize();
  led_drv_set_color(0, 0, 255);
  // blu_initialize();

  SENSOR_RESULT sensor_result = initialize(CONFIG_MAX_SENSOR_COUNT);
  if (sensor_result != SENSOR_SUCCESS) {
    ESP_LOGE(TAG, "Failed to initialize sensor-driver. Aborting");
    abort();
  }
  add_sensor(SENSOR_GPIO);

  relais_drv_initialize(PUMP_GPIO);

  sensor_reading reading;
  read_sensor_data(SENSOR_GPIO, &reading);
  ESP_LOGI(TAG, "Read %i from sensor");
  /* xTaskCreate(pump_task,   // Function that implements the task */
  /*             "Pump-Task", // Text name for the task (for debugging) */
  /*             2048,        // Stack size in bytes (not words, in ESP-IDF) */
  /*             NULL,        // Parameter passed into the task */
  /*             5,           // Task priority (higher number = higher priority)
   */
  /*             NULL // Task handle (use if you want to delete/suspend it
   * later) */
  /* ); */
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
  ui_start();
}
