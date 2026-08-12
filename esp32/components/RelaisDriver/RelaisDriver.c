#include "RelaisDriver.h"
#include "relais_register.h"
#include "relais_types.h"
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_log.h>

static const char *TAG = "RelaisDriver";

typedef enum relais_status_T {
  RELAIS_OPEN = OPEN_LEVEL,
  RELAIS_CLOSED = CLOSE_LEVEL
} relais_status_T;

/* Private methods */
esp_err_t set_relais_status(relais_handle_T handle, relais_status_T status) {
  relais_T relais;
  esp_err_t result = relais_register_fetch(handle, &relais);
  if (ESP_OK != result) {
    ESP_LOGE(TAG, "Relais with handle [%i] not found.");
    return ESP_FAIL;
  }

  ESP_ERROR_CHECK(gpio_set_level(relais.relais_gpio, status));
  if (status == RELAIS_OPEN)
    ESP_LOGI(TAG, "Opened relais [Handle: %i, GPIO: %i]", relais.handle,
             relais.relais_gpio);
  else
    ESP_LOGI(TAG, "Closed relais [Handle: %i, GPIO: %i]", relais.handle,
             relais.relais_gpio);

  return ESP_OK;
}

/* Public methods */
esp_err_t relais_drv_initialize(uint32_t initial_number) {
  relais_register_initialize(initial_number);
  return ESP_OK;
}

esp_err_t relais_drv_add_relais(uint32_t relais_gpio, relais_handle_T *handle) {
  relais_T new_relais = {.relais_gpio = relais_gpio};

  ESP_ERROR_CHECK(gpio_set_direction(relais_gpio, GPIO_MODE_OUTPUT));
  ESP_ERROR_CHECK(gpio_set_level(relais_gpio, CLOSE_LEVEL));

  ESP_ERROR_CHECK(relais_register_insert(new_relais, handle));
  ESP_LOGI(TAG, "Relais at GPIO %i successfully initialized. Initial state: %i",
           relais_gpio, CLOSE_LEVEL);
  return ESP_OK;
}

esp_err_t relais_drv_open(relais_handle_T handle) {
  return set_relais_status(handle, RELAIS_OPEN);
}

esp_err_t relais_drv_close(relais_handle_T handle) {
  return set_relais_status(handle, RELAIS_CLOSED);
}
