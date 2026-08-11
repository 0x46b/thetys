#include "PlantManager.h"
#include "RGBLEDDriver.h"
#include "thetys_ui.h"
#include "ui_data.h"
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

static const char *TAG = "app_main";

/* void pump_task(void *params) { */
/*   led_drv_set_color(0, 50, 0); */
/*   ESP_LOGI(TAG, "Starting pump for 30 seconds"); */
/*   relais_drv_open(); */
/*   vTaskDelay(pdMS_TO_TICKS(10000)); */
/*   ESP_LOGI(TAG, "Stopping pump"); */
/*   relais_drv_close(); */
/*   led_drv_set_color(50, 0, 0); */

/*   vTaskDelete(NULL); */
/* } */

void start_ui(void) {
  ui_lcd_gpio_config config = {.gpio_mosi = CONFIG_LCD_DIN,
                               .gpio_clk = CONFIG_LCD_CLK,
                               .gpio_cs = CONFIG_LCD_CS,
                               .gpio_dc = CONFIG_LCD_DC,
                               .gpio_rst = CONFIG_LCD_RST,
                               .gpio_bl = CONFIG_LCD_BL};
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

void app_main(void) {
  led_drv_initialize();
  led_drv_set_brightness(50);
  led_drv_set_to(CYAN);

  plant_mgr_initialize();

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
  plant_T test_plant =
      plant_mgr_create_plant("Testpflanze", PUMP_GPIO, SENSOR_GPIO, 50);
  plant_mgr_add_plant(&test_plant);
  plant_mgr_start();
  start_ui();
  led_drv_set_to(GREEN);
}
