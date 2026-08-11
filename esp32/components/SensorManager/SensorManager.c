/*! @file
 * API for managing sensors.
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
#include "SensorManager.h"
#include "SensorDriver.h"
#include "sensor_calibration.h"
#include "sensor_register.h"
#include "sensor_samples.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>
#include <sdkconfig.h>
#include <stdint.h>

static const char *TAG = "SensorManager";
static new_measurement_callback_t on_new_measurement;
static new_sensor_callback_t on_new_sensor;

SMGR_RESULT
sensor_mgr_subscribe_new_measurement(new_measurement_callback_t callback) {
  if (on_new_measurement != NULL) {
    ESP_LOGE(TAG, "Callback for new measurements is already assigned");
    return SMGR_CALLBACK_ALREADY_ASSIGNED;
  }
  on_new_measurement = callback;
  return SMGR_SUCCESS;
}

SMGR_RESULT
sensor_mgr_subscribe_new_sensor(new_sensor_callback_t callback) {
  on_new_sensor = callback;
  return SMGR_SUCCESS;
}

SMGR_RESULT sensor_mgr_initialize(uint32_t number_of_sensors) {
  SENSOR_REGISTER_RESULT result = sensor_register_initialize(number_of_sensors);
  ESP_ERROR_CHECK(sensor_data_initialize(number_of_sensors));
  switch (result) {
  case SENSOR_CFG_OK:
    ESP_LOGI(TAG, "Successfully initialized configurations for %i sensors",
             number_of_sensors);
    return SMGR_SUCCESS;
  case SENSOR_CFG_LOW_MEMORY:
    ESP_LOGE(TAG, "Could not initialize memory for sensor-configurations");
    return SMGR_CATASTROPHIC_FAILURE;
  default:
    ESP_LOGE(TAG, "Unknown error occured");
    return SMGR_UNKNOWN_ERROR;
  };
}

SMGR_RESULT sensor_manager_read_sensor(uint32_t sensor_handle,
                                       sensor_reading_T *reading) {
  if (reading == NULL) {
    ESP_LOGE(TAG, "Parameter 'reading' was not initialized");
    return SMGR_UNKNOWN_ERROR;
  }
  sensor_T sensor;
  if (sensor_register_fetch(sensor_handle, &sensor) != SENSOR_CFG_OK) {
    ESP_LOGE(TAG, "Could not retrieve configuration for sensor %i",
             sensor_handle);
    abort();
  }

  uint32_t raw_value;
  ESP_ERROR_CHECK(sensor_drv_read(sensor.sensor_gpio, &raw_value));

  reading->sensor_id = sensor_handle;
  reading->humidity_percentage =
      get_humidity(raw_value, sensor.calibration_data);

  ESP_LOGI(TAG, "Read humidity of %f percent from sensor %i",
           reading->humidity_percentage, sensor_handle);
  return SMGR_SUCCESS;
}

//*********** TODO: Replace hard-coded calibration! ************** //
SMGR_RESULT sensor_mgr_add_sensor(uint32_t sensor_gpio,
                                  sensor_handle_T *sensor_handle) {
  uint32_t number_of_sensors = 0;
  sensor_register_get_count(&number_of_sensors);
  sensor_T new_sensor = {
      .sensor_gpio = sensor_gpio,
      .calibration_data = {.calibrated = false,
                           .air_measurement = 4095,
                           .water_measurement = 2717,
                           .air_reference = CONFIG_SENSOR_AIR_REFERENCE_VALUE,
                           .water_reference =
                               CONFIG_SENSOR_WATER_REFERENCE_VALUE}};

  SENSOR_REGISTER_RESULT result =
      sensor_register_insert(new_sensor, sensor_handle);

  switch (result) {
  case SENSOR_CFG_OK:
    ESP_LOGI(TAG, "Successfully registered sensor [Handle: %i, GPIO: %i]",
             *sensor_handle, sensor_gpio);
    ESP_ERROR_CHECK(sensor_data_insert(0));
    sensor_drv_initialize(sensor_gpio);

    if (on_new_sensor != NULL) {
      on_new_sensor(new_sensor);
    }

    return SMGR_SUCCESS;
  case SENSOR_CFG_LOW_MEMORY:
    ESP_LOGE(TAG, "Could not add sensor: Not enough memory");
    return SMGR_CATASTROPHIC_FAILURE;
  default:
    ESP_LOGE(TAG,
             "Could not register sensor with GPIO %i: Unknown error occured",
             sensor_gpio);
    return SMGR_UNKNOWN_ERROR;
  }
}

static void sensor_polling_task(void *param) {
  sensor_reading_T value_buffer;
  uint32_t sensor_count = 0;
  if (SENSOR_CFG_OK != sensor_register_get_count(&sensor_count)) {
    ESP_LOGE(TAG, "Could not retrieve number of sensors");

    vTaskDelete(NULL);
    return;
  }

  if (sensor_count == 0) {
    ESP_LOGW(TAG, "No sensors found. Have you forgotten to add them? Ending "
                  "polling-task");
    vTaskDelete(NULL);
    return;
  }

  while (1) {
    ESP_LOGI(TAG, "Sensor polling started");

    for (uint32_t i = 0; i < sensor_count; i++) {
      ESP_LOGI(TAG, "Polling sensor %i/%i", i + 1, sensor_count);

      if (sensor_manager_read_sensor(i, &value_buffer) != SMGR_SUCCESS) {
        ESP_LOGE(TAG, "Error reading sensor %i", i);
        continue;
      }
      sensor_data_update(value_buffer.sensor_id,
                         value_buffer.humidity_percentage);

      if (on_new_measurement != NULL) {
        on_new_measurement(value_buffer);
      }
    }

    vTaskDelay(CONFIG_SENSOR_POLLING_TIME_IN_MS / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

SMGR_RESULT sensor_mgr_start_polling_task(void) {
  BaseType_t result;
  result =
      xTaskCreatePinnedToCore(sensor_polling_task, "Sensor polling", 4 * 1024,
                              NULL, 4, NULL, CONFIG_SENSOR_POLLING_CORE);
  if (result != pdPASS) {
    ESP_LOGE(TAG, "Could not start sensor_polling-task");
    return SENSOR_ERROR;
  }

  return SENSOR_SUCCESS;
}
