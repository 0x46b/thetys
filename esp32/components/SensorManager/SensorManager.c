#include "SensorManager.h"
#include "include/SensorManager.h"
#include "sensor_configuration.h"
#include <sdkconfig.h>
#include <stdint.h>

static sensor_configuration_data *sensor_configurations;

uint32_t get_next_free_id() { return sensor_configurations->last_id++; }

SMGR_RESULT initialize(uint32_t number_of_sensors) {
  SENSOR_CONFIGURATION_RESULT result =
      init_configurations(sensor_configurations, number_of_sensors);
  switch (result) {
  case SUCCESS:
    ESP_LOGI(SMGR_TAG, "Successfully initialized configurations for %i sensors",
             number_of_sensors);
    return SMGR_SUCCESS;
  case LOW_MEMORY:
    ESP_LOGE(SMGR_TAG, "Could not initialize memory for sensor-configurations");
    return SMGR_CATASTROPHIC_FAILURE;
  default:
    ESP_LOGE(SMGR_TAG, "Unknown error occured");
    return SMGR_UNKNOWN_ERROR;
  };
}

SMGR_RESULT read_sensor_data(uint32_t sensor_id, sensor_reading *reading) {
  if (reading == NULL) {
    ESP_LOGE(SMGR_TAG, "Parameter 'reading' was not initialized");
    return SMGR_UNKNOWN_ERROR;
  }

  reading->sensor_id = sensor_id;
  reading->humidity_percentage = 0.5;

  ESP_LOGI(SMGR_TAG, "Read humidity of %f percent from sensor %i", 0.5,
           sensor_id);
  return SMGR_SUCCESS;
}

SMGR_RESULT add_sensor(uint32_t sensor_pin) {
  uint32_t sensor_id = get_next_free_id();
  sensor_configuration new_sensor = {.sensor_id = sensor_id,
                                     .sensor_gpio = sensor_pin,
                                     .calibration_data = {
                                         .calibrated = false,
                                         .air_factor = 0,
                                         .water_factor = 0,
                                     }};

  SENSOR_CONFIGURATION_RESULT result =
      insert_configuration(sensor_configurations, new_sensor);

  switch (result) {
  case SUCCESS:
    sensor_configurations->last_id = sensor_id;
    ESP_LOGI(SMGR_TAG, "Successfully registered sensor with GPIO %i to Id %id",
             sensor_pin, sensor_id);
    return SMGR_SUCCESS;
  case LOW_MEMORY:
    ESP_LOGE(SMGR_TAG, "Could not add sensor: Not enough memory");
    return SMGR_CATASTROPHIC_FAILURE;
  default:
    ESP_LOGE(SMGR_TAG,
             "Could not register sensor with GPIO %i: Unknown error occured",
             sensor_pin);
    return SMGR_UNKNOWN_ERROR;
  }
}

SMGR_RESULT calibrate_air(uint32_t sensor_id) { return SMGR_SUCCESS; }

SMGR_RESULT calibrate_water(uint32_t sensor_id) { return SMGR_SUCCESS; }
