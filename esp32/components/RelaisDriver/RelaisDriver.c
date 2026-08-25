#include "RelaisDriver.h"
#include "relais_register.h"
#include "relais_types.h"
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_sleep.h>

static const char *TAG = "RelaisDriver";

static relais_fatal_callback_T on_fatal_callback;

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
    ESP_LOGD(TAG, "Opened relais [Handle: %i, GPIO: %i]", relais.handle,
             relais.relais_gpio);
  else
    ESP_LOGD(TAG, "Closed relais [Handle: %i, GPIO: %i]", relais.handle,
             relais.relais_gpio);

  return ESP_OK;
}

/* Public methods */
esp_err_t
relais_drv_subscribe_to_fatal_failure(relais_fatal_callback_T callback) {
  if (on_fatal_callback != NULL) {
    ESP_LOGE(TAG, "Callback for fatal relais-error already subscribed");
    return ESP_ERR_NOT_ALLOWED;
  }

  on_fatal_callback = callback;
  return ESP_OK;
}

esp_err_t relais_drv_initialize(uint32_t initial_number) {
  relais_register_initialize(initial_number);
  return ESP_OK;
}

esp_err_t relais_drv_add_relais(uint32_t relais_gpio, relais_handle_T *handle) {
  relais_T new_relais = {.relais_gpio = relais_gpio};

  ESP_ERROR_CHECK(gpio_set_direction(relais_gpio, GPIO_MODE_OUTPUT));
  ESP_ERROR_CHECK(gpio_set_level(relais_gpio, CLOSE_LEVEL));

  ESP_ERROR_CHECK(relais_register_insert(new_relais, handle));
  ESP_LOGD(TAG, "Relais at GPIO %i successfully initialized. Initial state: %i",
           relais_gpio, CLOSE_LEVEL);
  return ESP_OK;
}

esp_err_t relais_drv_open(relais_handle_T handle) {
  return set_relais_status(handle, RELAIS_OPEN);
}

esp_err_t relais_drv_close(relais_handle_T handle) {
  return set_relais_status(handle, RELAIS_CLOSED);
}

esp_err_t relais_drv_emergency_off() {
  relais_T relais;
  esp_err_t repository_result;
  bool error_occured = false;

  ESP_ERROR_CHECK(relais_register_reset_fetch());

  while (relais_register_fetch_next(&relais) == FETCH_OK) {
    repository_result = set_relais_status(relais.handle, RELAIS_CLOSED);

    if (repository_result != ESP_OK) {
      ESP_LOGE(TAG, "Could not close relais %i!", relais.handle);
      error_occured = true;
    }
  }

  if (error_occured) {
    ESP_LOGE(TAG, "Emergency shutoff has failed for one or more relais");
    if (on_fatal_callback != NULL) {
      on_fatal_callback();
    }
    return ESP_FAIL;
  }
  return ESP_OK;
}
