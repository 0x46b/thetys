#include "PlantManager.h"
#include "LEDManager.h"
#include "RelaisDriver.h"
#include "SensorDriver.h"
#include "SensorManager.h"
#include "plant_mgr_type_definitions.h"
#include "plant_repository.h"
#include "plant_service.h"
#include "relais_types.h"
#include "sensor_data_task.h"
#include "sensor_mgr_type_definitions.h"
#include "sensor_reading_task.h"
#include <esp_err.h>
#include <esp_log.h>
#include <esp_sleep.h>
#include <freertos/FreeRTOS.h>
#include <portmacro.h>

#define DEFAULT_PUMP_TIME_IN_MS 6000
#define TASK_STACK_DEPTH 4 * 1024
#define TASK_PRIORITY 4

static const char *TAG = "PlantManager";
static plant_repository_T plants;

/* Private functions */
void print_plant(plant_T plant) {
  ESP_LOGI(
      TAG,
      "Plant: {Name = '%s', Handle: %i, Pump Handle: %i, Sensor Handle: %i}",
      plant.plant_name, plant.handle, plant.pump_handle, plant.sensor_handle);
}

esp_err_t load_plants() {
  plant_repo_load_plants(&plants);
  return ESP_OK;
}

void on_fatal_relais_fail(void) {
  ESP_LOGE(TAG, "Relais failed without recovery, intiating emergency system "
                "shutdown. Press the reset-button to wake up.");
  led_context_enter(CTX_FATAL);
  esp_deep_sleep_start();
}

esp_err_t set_pump_state(relais_handle_T handle, pump_state_T state) {
  esp_err_t relais_result;
  switch (state) {
  case PUMP_TIMED:
    relais_result = relais_drv_open(handle);
    vTaskDelay(DEFAULT_PUMP_TIME_IN_MS / portTICK_PERIOD_MS);
    relais_drv_close(handle);
    break;

  case PUMP_ON:
    led_context_enter(CTX_BUSY);
    relais_result = relais_drv_open(handle);
    break;
  case PUMP_OFF:
    led_context_leave(CTX_BUSY);
    relais_result = relais_drv_close(handle);
    break;
  default:
    ESP_LOGE(TAG, "Could not set pump to unknown status! %i", state);
    return ESP_FAIL;
  }

  // Something went wrong, at least try to close the pump...
  if (relais_result != ESP_OK) {
    led_context_enter(CTX_ERROR);
    ESP_LOGE(TAG,
             "Error occured while setting pump-state for pump %i: '%s'\n. "
             "Trying to shutting it off...",
             handle, esp_err_to_name(relais_result));
    if (relais_drv_close(handle) != ESP_OK) {
      ESP_LOGE(TAG, "Ok, shit had hit the fan. Initializing emergency shutoff "
                    "of all pumps...");
      relais_drv_emergency_off();
    }
  }

  return relais_result;
}

esp_err_t plant_mgr_set_pump_state(plant_handle_T handle, pump_state_T state) {
  plant_T plant;
  ESP_ERROR_CHECK(plant_repo_fetch(handle, &plant));
  if (state == PUMP_TIMED) {
    set_pump_state(plant.pump_handle, state);
  }

  return ESP_OK;
}

/*! @todo This should be a separate task, so we do not block something */
esp_err_t plant_mgr_set_pump_on_for(plant_handle_T handle,
                                    uint32_t time_in_ms) {
  plant_T plant;
  ESP_ERROR_CHECK(plant_repo_fetch(handle, &plant));
  led_context_enter(CTX_BUSY);

  ESP_LOGI(TAG, "Setting pump %i on for %ims", plant.pump_handle, time_in_ms);
  set_pump_state(plant.pump_handle, PUMP_ON);
  vTaskDelay(time_in_ms / portTICK_PERIOD_MS);
  set_pump_state(plant.pump_handle, PUMP_OFF);

  led_context_leave(CTX_BUSY);
  ESP_LOGI(TAG, "Setting pump %i off", plant.pump_handle);
  return ESP_OK;
}

esp_err_t start_plant_tasks(plant_T *plant) {
  ESP_ERROR_CHECK(sensor_reading_task_create(plant));
  ESP_ERROR_CHECK(sensor_data_saving_task_create(plant));
  return ESP_OK;
}

/* Public functions */
esp_err_t plant_mgr_initialize() {
  led_context_init();
  led_context_enter(CTX_BUSY);
  ESP_ERROR_CHECK(plant_repo_initialize());
  ESP_ERROR_CHECK(load_plants());
  ESP_ERROR_CHECK(relais_drv_initialize(MAX_PLANTS));
  ESP_ERROR_CHECK(relais_drv_subscribe_to_fatal_failure(on_fatal_relais_fail));
  SENSOR_RESULT result = sensor_mgr_initialize(MAX_PLANTS);
  if (SENSOR_SUCCESS != result) {
    /* @todo better error-handling */
    ESP_LOGE(TAG, "Could not start SensorManager");
    return ESP_FAIL;
  }

  /* BLU_RESULT blu_result = blu_initialize(); */
  /* if (BLU_OK != blu_result) { */
  /*   /\* @todo better error-handling *\/ */
  /*   ESP_LOGE(TAG, "Callback is already assigned!"); */
  /*   return ESP_FAIL; */
  /* } */

  /* BLU_RESULT blu_subscribe_result = */
  /*     blue_subscribe_new_plant(bluetooth_new_plant_handler); */
  /* if (BLU_OK != blu_subscribe_result) { */
  /*   /\* @todo better error-handling *\/ */
  /*   ESP_LOGE(TAG, "Callback is already assigned!"); */
  /*   return ESP_FAIL; */
  /* } */

  led_context_leave(CTX_BUSY);
  return ESP_OK;
}

esp_err_t plant_mgr_add_plant(const char *plant_name, uint32_t pump_gpio,
                              uint32_t sensor_gpio, uint32_t watering_threshold,
                              plant_T *created_plant) {
  created_plant->plant_name = plant_name;
  created_plant->watering_threshold = watering_threshold;
  created_plant->is_active = true;

  sensor_handle_T sensor_handle;
  SENSOR_RESULT sensor_result =
      sensor_mgr_add_sensor(sensor_gpio, &sensor_handle);
  if (SENSOR_SUCCESS != sensor_result) {
    ESP_LOGE(TAG, "Failed to register sensor with GPIO %i");
    return ESP_FAIL;
  }
  created_plant->sensor_handle = sensor_handle;

  relais_handle_T relais_handle;
  esp_err_t relais_result = relais_drv_add_relais(pump_gpio, &relais_handle);
  if (ESP_OK != relais_result) {
    ESP_LOGE(TAG, "Could not add pump at GPIO %i", pump_gpio);
    return ESP_FAIL;
  }
  created_plant->pump_handle = relais_handle;
  print_plant(*created_plant);
  start_plant_tasks(created_plant);

  return ESP_OK;
}

esp_err_t plant_mgr_remove_plant(plant_T *remove_plant) { return ESP_OK; }

esp_err_t plant_mgr_set_plant_status(plant_handle_T handle, bool is_active) {
  plant_T plant;

  ESP_ERROR_CHECK(plant_repo_fetch(handle, &plant));
  plant.is_active = is_active;
  ESP_ERROR_CHECK(plant_repo_update(plant));

  return ESP_OK;
}

esp_err_t plant_mgr_get_humidity(plant_handle_T plant,
                                 humidity_data_T *humidity) {
  return ESP_OK;
}
