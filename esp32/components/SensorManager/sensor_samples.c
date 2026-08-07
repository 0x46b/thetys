#include "sensor_samples.h"
#include "esp_err.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char *TAG = "Sensor Samples";

static SemaphoreHandle_t sample_mutex;
static sensor_sample_array sensor_samples;

esp_err_t sensor_data_initialize(uint32_t initialSize) {
  if (sensor_samples.samples != NULL) {
    ESP_LOGW(TAG,
             "Duplicate call to sensor_data_initialize(). Ignoring this one");
    return ESP_ERR_NOT_ALLOWED;
  }

  ESP_LOGI(TAG, "Initializing array for %i samples", initialSize);
  // If reset was called and the array gets reinitialized this could happen, so
  // if samples is NULL but we already have a mutex, we can reuse it
  if (sample_mutex == NULL) {
    sample_mutex = xSemaphoreCreateMutex();
  }

  xSemaphoreTake(sample_mutex, portMAX_DELAY);
  sensor_samples.samples = malloc(initialSize * sizeof(uint32_t));

  if (sensor_samples.samples == NULL) {
    ESP_LOGE(TAG, "Malloc failed!");
    return ESP_ERR_NO_MEM;
  }

  sensor_samples.used = 0;
  sensor_samples.size = initialSize;

  xSemaphoreGive(sample_mutex);
  ESP_LOGI(TAG, "Successfully initialized memory for %i samples.", initialSize);
  return ESP_OK;
}

esp_err_t sensor_data_insert(uint32_t sample) {
  xSemaphoreTake(sample_mutex, portMAX_DELAY);
  if (sensor_samples.samples == NULL) {
    ESP_LOGE(TAG, "Try to insert into uninitialized array");
    return ESP_OK;
  }

  if (sensor_samples.used == sensor_samples.size) {
    uint32_t new_size = sensor_samples.size * 2;

    ESP_LOGI(TAG,
             "Not enough space for adding new sample, resizing from %i to %i",
             sensor_samples.size, new_size);
    uint32_t *new_array =
        realloc(sensor_samples.samples, sensor_samples.size * sizeof(uint32_t));
    if (new_array == NULL) {
      ESP_LOGE(TAG, "Resizing failed");
      return ESP_ERR_NO_MEM;
    }
    sensor_samples.size = new_size;
    sensor_samples.samples = new_array;
  }

  sensor_samples.samples[sensor_samples.used++] = sample;

  xSemaphoreGive(sample_mutex);
  ESP_LOGI(TAG, "Added sample '%i'. Now storing %i samples", sample,
           sensor_samples.used);
  return ESP_OK;
}

esp_err_t sensor_data_update(uint32_t id, uint32_t sample) {
  xSemaphoreTake(sample_mutex, portMAX_DELAY);

  if (id > sensor_samples.used) {
    ESP_LOGE(TAG, "Try to insert with id '%i', but max-id is '%i'", id,
             sensor_samples.used);
    return ESP_ERR_NOT_SUPPORTED;
  }
  sensor_samples.samples[id] = sample;

  xSemaphoreGive(sample_mutex);
  ESP_LOGI(TAG, "Updated sample for sensor '%i' to '%i'", id, sample);
  return ESP_OK;
}

esp_err_t sensor_data_reset() {
  xSemaphoreTake(sample_mutex, portMAX_DELAY);

  free(sensor_samples.samples);
  sensor_samples.samples = NULL;
  sensor_samples.used = 0;
  sensor_samples.size = 0;

  xSemaphoreGive(sample_mutex);
  ESP_LOGI(TAG, "Samples reset");
  return ESP_OK;
}
