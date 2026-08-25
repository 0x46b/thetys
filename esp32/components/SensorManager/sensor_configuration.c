/* API for managing sensor-configurations.
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
#include "include/sensor_configuration.h"
#include "sensor_mgr_type_definitions.h"
#include "sensor_register.h"
#include <stdint.h>
#include <stdlib.h>

static const char *TAG = "sensor-register";
static sensor_configuration_list_T sensor_configurations;
static bool initialized = false;

SENSOR_REGISTER_RESULT
sensor_register_initialize(size_t initialSize) {
  if (initialSize == 0) {
    ESP_LOGE(TAG, "Initializing with 0 elements is not allowed.");
    return SENSOR_CFG_INITIALIZATION_ERROR;
  }

  ESP_LOGI(TAG, "Initializing array for %i sensors.", initialSize);
  sensor_configurations.configurations =
      malloc(initialSize * sizeof(sensor_configuration_T));

  if (sensor_configurations.configurations == NULL) {
    ESP_LOGE(TAG, "Malloc failed!");
    return SENSOR_CFG_LOW_MEMORY;
  }

  sensor_configurations.used = 0;
  sensor_configurations.size = initialSize;
  initialized = true;

  ESP_LOGI(TAG, "Successfully initialized memory for %i configurations.",
           initialSize);
  return SENSOR_CFG_OK;
}

SENSOR_REGISTER_RESULT
sensor_register_insert(sensor_configuration_T element,
                       sensor_handle_T *handle) {
  if (!initialized) {
    ESP_LOGE(TAG, "Try to insert into uninitialized array");
    return SENSOR_CFG_UNINITIALIZED;
  }

  if (sensor_configurations.used == sensor_configurations.size) {
    uint32_t new_size = sensor_configurations.size * 2;

    ESP_LOGI(TAG,
             "Not enough space for added sensor_config, resizing from %i to %i",
             sensor_configurations.size, new_size);
    sensor_configuration_T *new_array =
        realloc(sensor_configurations.configurations,
                new_size * sizeof(sensor_configuration_T));
    if (new_array == NULL) {
      ESP_LOGE(TAG, "Resizing failed");
      return SENSOR_CFG_LOW_MEMORY;
    }
    sensor_configurations.size = new_size;
    sensor_configurations.configurations = new_array;
  }

  uint32_t new_index = sensor_configurations.used;
  sensor_configurations.configurations[new_index] = element;
  *handle = new_index;

  ESP_LOGI(
      TAG,
      "Inserted new sensor-configuration [Handle: %i, GPIO %i](%i used total)",
      *handle, element.sensor_gpio, sensor_configurations.used);

  return SENSOR_CFG_OK;
}

SENSOR_REGISTER_RESULT
sensor_register_free() {
  free(sensor_configurations.configurations);
  sensor_configurations.configurations = NULL;
  sensor_configurations.used = 0;
  sensor_configurations.size = 0;
  initialized = false;

  ESP_LOGI(TAG, "Reset configurations");
  return SENSOR_CFG_OK;
}

SENSOR_REGISTER_RESULT
sensor_register_fetch(sensor_configuration_T *sensor_config,
                      sensor_handle_T sensor_handle) {
  if (sensor_handle > sensor_configurations.used) {
    ESP_LOGE(TAG, "Sensor with id %i is not known", sensor_handle);
    return SENSOR_CFG_UNKNOWN_SENSOR_ID;
  }
  sensor_configuration_T stored_config =
      sensor_configurations.configurations[(uint32_t)sensor_handle];
  *sensor_config = stored_config;

  ESP_LOGI(TAG, "Returning config for sensor %i (gpio:%i, air: %i, water:%i)",
           sensor_handle, stored_config.sensor_gpio,
           stored_config.calibration_data.air_measurement,
           stored_config.calibration_data.water_measurement);
  return SENSOR_CFG_OK;
}

SENSOR_REGISTER_RESULT get_number_of_configurations(uint32_t *count) {
  if (count == NULL) {
    ESP_LOGE(TAG, "count is null");
    return SENSOR_CFG_UNINITIALIZED;
  }

  *count = sensor_configurations.used;
  return SENSOR_CFG_OK;
}
