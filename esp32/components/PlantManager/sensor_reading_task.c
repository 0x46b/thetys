#include "sensor_reading_task.h"
#include "SensorManager.h"
#include "freertos/idf_additions.h"
#include "plant_mgr_type_definitions.h"
#include "sensor_mgr_type_definitions.h"
#include <esp_err.h>
#include <esp_log.h>
#include <sdkconfig.h>
#include <stdlib.h>
#include <string.h>

const char *TAG = "PlantManager.plant_polling_task";

/* Private methods*/
time_t get_timestamp() {
  time_t now;
  struct tm timeinfo;

  time(&now);
  setenv("DE", "", 1);
  tzset();

  localtime_r(&now, &timeinfo);
  return now;
}

static void plant_polling_task(void *param) {
  plant_T *plant = (plant_T *)param;

  if (plant == NULL) {
    ESP_LOGE(TAG, "plant is NULL, aborting task");
    vTaskDelete(NULL);
    return;
  }
  char *plant_name = strdup(plant->plant_name);

  if (plant_name == NULL) {
    ESP_LOGW(TAG, "Could not allocate memory for the plant-name");
  }
  plant_handle_T plant_handle = plant->handle;
  sensor_handle_T sensor_handle = plant->sensor_handle;
  QueueHandle_t queue_handle = plant->data_queue_handle;

  if (queue_handle == NULL) {
    ESP_LOGE(TAG, "Queue-Handle for plant '%s' is NULL", plant_name);
    free(plant_name);
    vTaskDelete(NULL);
    return;
  }

  sensor_reading_T value_buffer;
  while (1) {
    ESP_LOGI(TAG, "Sensor polling for plant '%s' started", plant_name);

    if (sensor_manager_read_sensor(sensor_handle, &value_buffer) !=
        SMGR_SUCCESS) {
      ESP_LOGE(TAG, "Error reading sensor %i", sensor_handle);
      vTaskDelay(pdMS_TO_TICKS(CONFIG_SENSOR_POLLING_TIME_IN_MS));
      continue;
    }
    sensor_entry_T entry = {.plant = plant_handle,
                            .humidity_percentage =
                                value_buffer.humidity_percentage,
                            .timestamp = get_timestamp()};
    xQueueSend(queue_handle, (void *)&entry, pdMS_TO_TICKS(1000));

    vTaskDelay(pdMS_TO_TICKS(CONFIG_SENSOR_POLLING_TIME_IN_MS));
  }
  free(plant_name);
  vTaskDelete(NULL);
}

/* Public methods */
esp_err_t sensor_reading_task_create(plant_T *plant) {
  ESP_LOGI(TAG, "Starting sensor reading for plant '%s'(Sensorhndl:%i)",
           plant->plant_name, plant->sensor_handle);
  char task_name[30];
  sprintf(task_name, "Plant Polling %s", plant->plant_name);

  plant->data_queue_handle =
      xQueueCreate(CONFIG_READING_TASK_BUFFER_SIZE, sizeof(sensor_entry_T));
  if (plant->data_queue_handle == NULL) {
    ESP_LOGE(TAG, "Could not create queue for plant %i", plant->handle);
    return ESP_FAIL;
  }

  BaseType_t result = xTaskCreatePinnedToCore(
      plant_polling_task, task_name, CONFIG_READING_TASK_STACK_DEPTH, plant,
      CONFIG_READING_TASK_PRIORITY, &(plant->reading_task_handle),
      CONFIG_SENSOR_POLLING_CORE);
  if (result != pdPASS) {
    ESP_LOGE(TAG, "Could not start sensor-reading-task for plant %s",
             plant->plant_name);
    return ESP_FAIL;
  }
  return ESP_OK;
}
