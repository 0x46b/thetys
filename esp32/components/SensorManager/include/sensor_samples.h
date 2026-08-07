#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

// Force ESP32 architecture definitions to settle first
#include <esp_err.h>
#include <stdint.h>

typedef struct sensor_sample_array {
  uint32_t *samples;
  uint32_t size;
  uint32_t used;
} sensor_sample_array;

esp_err_t get_data_for_sensor(uint32_t sensor_id, uint32_t *sample);
esp_err_t sensor_data_initialize(uint32_t size);
esp_err_t sensor_data_insert(uint32_t sample);
esp_err_t sensor_data_update(uint32_t id, uint32_t sample);
esp_err_t sensor_data_reset();

#endif // SENSOR_DATA_H
