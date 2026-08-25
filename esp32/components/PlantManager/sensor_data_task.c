#include "sensor_data_task.h"
#include "LEDManager.h"
#include "portmacro.h"
#include "sdkconfig.h"
#include "sensor_reading_task.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>

static const char *TAG = "PlantManager.plant_data_saving";

/* Private methods*/
static void save_plant_data(sensor_entry_T data) {
  ESP_LOGI(TAG, "Simulated saving of data");
}

static void handle_watering(sensor_entry_T data, uint32_t watering_threshold) {
  if (data.humidity_percentage < watering_threshold) {
    ESP_LOGI(TAG, "Simulated WATER ON");
  }
}

static void plant_data_handling_task(void *param) {
  plant_T *plant = (plant_T *)param;

  if (plant == NULL) {
    ESP_LOGW(TAG, "plant is NULL, aborting task");
    vTaskDelete(NULL);
    return;
  }

  QueueHandle_t queue_handle = plant->data_queue_handle;

  sensor_entry_T entry_buffer;
  BaseType_t result;
  char *plant_name = strdup(plant->plant_name);
  uint32_t items_in_queue = 0;
  uint32_t watering_threshold = plant->watering_threshold;

  if (queue_handle == NULL) {
    ESP_LOGE(TAG, "Queue handle is NULL, aborting...");
    free(plant_name);
    vTaskDelete(NULL);
  }

  while (1) {
    led_context_enter(CTX_BUSY);
    ESP_LOGD(TAG, "Sensor data task for plant '%s' started", plant_name);
    items_in_queue = uxQueueMessagesWaiting(queue_handle);
    ESP_LOGD(TAG, "%i items waiting in the queue", items_in_queue);

    result = xQueueReceive(queue_handle, &entry_buffer, pdMS_TO_TICKS(1000));
    if (result == errQUEUE_EMPTY) {
      ESP_LOGW(TAG, "Queue is empty");
      vTaskDelay(pdMS_TO_TICKS(CONFIG_SENSOR_POLLING_TIME_IN_MS));
      continue;
    }
    ESP_LOGD(TAG,
             "Received data: {Humidity: %i\%, Plant_Handle: %i, Timestamp: %i}",
             entry_buffer.humidity_percentage, entry_buffer.plant,
             entry_buffer.timestamp);

    save_plant_data(entry_buffer);
    handle_watering(entry_buffer, watering_threshold);

    led_context_leave(CTX_BUSY);
    vTaskDelay(pdMS_TO_TICKS(CONFIG_SENSOR_POLLING_TIME_IN_MS));
  }
  free(plant_name);
  vTaskDelete(NULL);
}

/* Public methods */
esp_err_t sensor_data_saving_task_create(plant_T *plant) {
  ESP_LOGI(TAG, "Starting data handling for plant '%s'", plant->plant_name);
  char task_name[30];
  sprintf(task_name, "Data Handling %s", plant->plant_name);

  BaseType_t result = xTaskCreatePinnedToCore(
      plant_data_handling_task, task_name, CONFIG_DATA_TASK_STACK_DEPTH, plant,
      CONFIG_DATA_TASK_PRIORITY, &(plant->data_task_handle),
      CONFIG_SENSOR_POLLING_CORE);
  if (result != pdPASS) {
    ESP_LOGE(TAG, "Could not start data-handling-task for plant %s",
             plant->plant_name);
    return ESP_FAIL;
  }
  return ESP_OK;
}
