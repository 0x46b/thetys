#include "RelaisDriver.h"
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_log.h>

static uint32_t relais_pin;
static bool is_initialized;
static const char *TAG = "RelaisDriver";

void relais_drv_initialize(uint32_t pin) {
  relais_pin = pin;
  ESP_ERROR_CHECK(gpio_set_direction(relais_pin, GPIO_MODE_OUTPUT));
  is_initialized = true;
  ESP_ERROR_CHECK(gpio_set_level(relais_pin, CLOSE_LEVEL));
  ESP_LOGI(TAG, "Relais at GPIO %i successfully initialized. Initial state: %i",
           relais_pin, CLOSE_LEVEL);
}

void relais_drv_open(void) {
  if (!is_initialized) {
    ESP_LOGE(
        TAG,
        "RelaisDriver not initialized! Call relais_drv_initialize() first");
    return;
  }
  ESP_ERROR_CHECK(gpio_set_level(relais_pin, OPEN_LEVEL));
  ESP_LOGI(TAG, "Opened relais at %i", relais_pin);
}

void relais_drv_close(void) {
  if (!is_initialized) {
    ESP_LOGE(
        TAG,
        "RelaisDriver not initialized! Call relais_drv_initialize() first");
    return;
  }

  ESP_ERROR_CHECK(gpio_set_level(relais_pin, CLOSE_LEVEL));
  ESP_LOGI(TAG, "Closed relais at %i", relais_pin);
}
