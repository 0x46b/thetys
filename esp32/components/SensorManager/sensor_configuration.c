#include "sensor_configuration.h"
#include "include/sensor_configuration.h"
#include <stdlib.h>

SENSOR_CONFIGURATION_RESULT
init_configurations(sensor_configuration_data *configs, size_t initialSize) {
  ESP_LOGI(TAG, "Initializing array for %i sensors.", initialSize);
  sensor_configuration *new_array =
      malloc(initialSize * sizeof(sensor_configuration));
  if (new_array == NULL) {
    ESP_LOGE(TAG, "Malloc failed!");
    return LOW_MEMORY;
  }

  configs->configurations = new_array;
  configs->used = 0;
  configs->size = initialSize;

  ESP_LOGI(TAG, "Successfully initialized memory for %i configurations.",
           initialSize);
  return SUCCESS;
}

SENSOR_CONFIGURATION_RESULT
insert_configuration(sensor_configuration_data *configs,
                     sensor_configuration element) {
  if (configs->configurations == NULL) {
    ESP_LOGE(TAG, "Try to insert into uninitialized array");
    return UNINITIALIZED;
  }

  if (configs->used == configs->size) {
    uint32_t new_size = configs->size * 2;

    ESP_LOGI(TAG,
             "Not enough space for added sensor_config, resizing from %i to %i",
             configs->size, new_size);
    sensor_configuration *new_array = realloc(
        configs->configurations, configs->size * sizeof(sensor_configuration));
    if (new_array == NULL) {
      ESP_LOGE(TAG, "Resizing failed");
      return LOW_MEMORY;
    }
    configs->size = new_size;
    configs->configurations = new_array;
  }

  configs->configurations[configs->used++] = element;
}

SENSOR_CONFIGURATION_RESULT
free_configurations(sensor_configuration_data *configs) {
  free(configs->configurations);
  configs->configurations = NULL;
  configs->used = 0;
  configs->size = 0;
  ESP_LOGI(TAG, "Reset configurations");
}
