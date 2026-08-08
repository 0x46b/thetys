#include "SensorManager.h"
#include "SensorDriver.h"
#include "sensor_configuration.h"
#include "sensor_samples.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <sdkconfig.h>
#include <stdint.h>

static const char *TAG = "SensorManager";
static sensor_configuration_data sensor_configurations;
static void (*new_data_callback)(uint32_t, uint32_t);

SMGR_RESULT sensor_mgr_subscribe(void (*callback)(uint32_t, uint32_t)) {
  new_data_callback = callback;
  return SMGR_SUCCESS;
}

SMGR_RESULT sensor_mgr_initialize(uint32_t number_of_sensors) {
  SENSOR_CONFIGURATION_RESULT result =
      init_configurations(&sensor_configurations, number_of_sensors);
  ESP_ERROR_CHECK(sensor_data_initialize(number_of_sensors));
  switch (result) {
  case SUCCESS:
    ESP_LOGI(TAG, "Successfully initialized configurations for %i sensors",
             number_of_sensors);
    return SMGR_SUCCESS;
  case LOW_MEMORY:
    ESP_LOGE(TAG, "Could not initialize memory for sensor-configurations");
    return SMGR_CATASTROPHIC_FAILURE;
  default:
    ESP_LOGE(TAG, "Unknown error occured");
    return SMGR_UNKNOWN_ERROR;
  };
}

uint32_t get_humidity(uint32_t raw_value) {
  return (4095 - raw_value) / 4095 * raw_value;
}

SMGR_RESULT sensor_manager_read_sensor(uint32_t sensor_id,
                                       sensor_reading *reading) {
  if (reading == NULL) {
    ESP_LOGE(TAG, "Parameter 'reading' was not initialized");
    return SMGR_UNKNOWN_ERROR;
  }

  uint32_t raw_value;
  ESP_ERROR_CHECK(get_data_for_sensor(sensor_id, &raw_value));
  reading->sensor_id = sensor_id;
  reading->humidity_percentage = get_humidity(raw_value);

  ESP_LOGI(TAG, "Read humidity of %f percent from sensor %i",
           reading->humidity_percentage, sensor_id);
  return SMGR_SUCCESS;
}

SMGR_RESULT sensor_mgr_add_sensor(uint32_t sensor_gpio) {
  uint32_t sensor_id = sensor_configurations.used;
  sensor_configuration new_sensor = {.sensor_id = sensor_id,
                                     .sensor_gpio = sensor_gpio,
                                     .calibration_data = {
                                         .calibrated = false,
                                         .air_factor = 0,
                                         .water_factor = 0,
                                     }};

  SENSOR_CONFIGURATION_RESULT result =
      insert_configuration(&sensor_configurations, new_sensor);
  ESP_ERROR_CHECK(sensor_data_insert(0));
  sensor_drv_initialize(sensor_gpio);

  switch (result) {
  case SUCCESS:
    ESP_LOGI(TAG, "Successfully registered sensor with GPIO %i to Id %i",
             sensor_gpio, sensor_id);
    return SMGR_SUCCESS;
  case LOW_MEMORY:
    ESP_LOGE(TAG, "Could not add sensor: Not enough memory");
    return SMGR_CATASTROPHIC_FAILURE;
  default:
    ESP_LOGE(TAG,
             "Could not register sensor with GPIO %i: Unknown error occured",
             sensor_gpio);
    return SMGR_UNKNOWN_ERROR;
  }
}

SMGR_RESULT calibrate_air(uint32_t sensor_id) { return SMGR_SUCCESS; }

SMGR_RESULT calibrate_water(uint32_t sensor_id) { return SMGR_SUCCESS; }

static void sensor_polling_task(void *param) {
  uint32_t value_buffer;
  uint32_t current_sensor_gpio;
  if (sensor_configurations.used == 0) {
    ESP_LOGW(TAG, "No sensors found. Have you forgotten to add them? Ending "
                  "polling-task");
    vTaskDelete(NULL);
    return;
  }

  while (1) {
    ESP_LOGI(TAG, "Sensor polling started");

    for (uint32_t i = 0; i < sensor_configurations.used; i++) {
      ESP_LOGI(TAG, "Polling sensor %i/%i", i, sensor_configurations.used);

      current_sensor_gpio = sensor_configurations.configurations[i].sensor_gpio;
      if (sensor_drv_read(current_sensor_gpio, &value_buffer) !=
          SENSOR_SUCCESS) {
        ESP_LOGE(TAG, "Error reading sensor at GPIO %i", current_sensor_gpio);
        continue;
      }
      sensor_data_update(i, value_buffer);

      if (new_data_callback != NULL) {
        new_data_callback(i, value_buffer);
      }
    }

    vTaskDelay(CONFIG_SENSOR_POLLING_TIME_IN_MS / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

SMGR_RESULT start_sensor_polling_task(void) {
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
