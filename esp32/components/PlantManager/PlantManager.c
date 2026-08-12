#include "PlantManager.h"
#include "BluetoothManager.h"
#include "RelaisDriver.h"
#include "SensorDriver.h"
#include "SensorManager.h"
#include "plant_mgr_type_definitions.h"
#include "plant_repository.h"
#include "relais_types.h"
#include "sensor_mgr_type_definitions.h"
#include <esp_err.h>
#include <esp_log.h>

static const char *TAG = "PlantManager";
static plant_repository_T plants;

/* Private functions */
esp_err_t load_plants() {
  plant_repo_load_plants(&plants);
  return ESP_OK;
}

/* Callbacks */
void handle_new_measurement(sensor_reading_T new_measurement) {
  /* measurement new_measurement = {.sensor_id = reading.sensor_id, */
  /*                                 .timestamp = reading.timestamp, */
  /*                                 .humidity_in_percent = */
  /*                                     reading.humidity_percentage}; */
  /*  ui_add_measurement(new_measurement);   */
}

static void bluetooth_new_plant_handler(const char *plant_name,
                                        uint32_t sensor_gpio,
                                        uint32_t pump_gpio,
                                        uint32_t watering_threshold) {
  plant_T new_plant;
  esp_err_t result = plant_mgr_add_plant(plant_name, sensor_gpio, pump_gpio,
                                         watering_threshold, &new_plant);
  if (ESP_OK != result) {
    ESP_LOGE(TAG, "Could not create new plant");
    // Well, fuck. What we want to do now? :/
  }
}

/* Public functions */
esp_err_t plant_mgr_initialize() {
  ESP_ERROR_CHECK(plant_repo_initialize());
  ESP_ERROR_CHECK(load_plants());
  ESP_ERROR_CHECK(relais_drv_initialize(MAX_PLANTS));

  SENSOR_RESULT result = sensor_mgr_initialize(MAX_PLANTS);
  if (SENSOR_SUCCESS != result) {
    /* @todo better error-handling */
    ESP_LOGE(TAG, "Could not start SensorManager");
    return ESP_FAIL;
  }

  SENSOR_RESULT subscribe_result =
      sensor_mgr_subscribe_new_measurement(handle_new_measurement);
  if (SENSOR_SUCCESS != subscribe_result) {
    /* @todo better error-handling */
    ESP_LOGE(TAG, "Callback is already assigned!");
    return ESP_FAIL;
  }

  BLU_RESULT blu_result = blu_initialize();
  if (BLU_OK != blu_result) {
    /* @todo better error-handling */
    ESP_LOGE(TAG, "Callback is already assigned!");
    return ESP_FAIL;
  }

  BLU_RESULT blu_subscribe_result =
      blue_subscribe_new_plant(bluetooth_new_plant_handler);
  if (BLU_OK != blu_subscribe_result) {
    /* @todo better error-handling */
    ESP_LOGE(TAG, "Callback is already assigned!");
    return ESP_FAIL;
  }

  return ESP_OK;
}

esp_err_t plant_mgr_add_plant(const char *plant_name, uint32_t pump_gpio,
                              uint32_t sensor_gpio, uint32_t watering_threshold,
                              plant_T *created_plant) {
  plant_T new_plant = {.plant_name = plant_name,
                       .watering_threshold = watering_threshold,
                       .is_active = true};

  sensor_handle_T sensor_handle;
  SENSOR_RESULT sensor_result =
      sensor_mgr_add_sensor(sensor_gpio, &sensor_handle);
  if (SENSOR_SUCCESS != sensor_result) {
    ESP_LOGE(TAG, "Failed to register sensor with GPIO %i");
    return ESP_FAIL;
  }
  new_plant.sensor_handle = sensor_handle;

  relais_handle_T relais_handle;
  esp_err_t relais_result = relais_drv_add_relais(pump_gpio, &relais_handle);
  if (ESP_OK != relais_result) {
    ESP_LOGE(TAG, "Could not add pump at GPIO %i", pump_gpio);
    return ESP_FAIL;
  }
  new_plant.pump_handle = relais_handle;

  ESP_ERROR_CHECK(plant_repo_insert(new_plant));
  if (created_plant != NULL) {
    *created_plant = new_plant;
  }

  return ESP_OK;
}
esp_err_t plant_mgr_remove_plant(plant_T *remove_plant) { return ESP_OK; }
esp_err_t plant_mgr_start() { return ESP_OK; }

esp_err_t plant_mgr_set_plant_status(plant_handle_T handle, bool is_active) {
  plant_T plant;

  ESP_ERROR_CHECK(plant_repo_fetch(handle, &plant));
  plant.is_active = is_active;
  ESP_ERROR_CHECK(plant_repo_update(plant));

  return ESP_OK;
}
