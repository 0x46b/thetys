#include "SensorManager.h"
#include <esp_log.h>
#include <sdkconfig.h>

static const char *TAG = "MAIN";

void app_main(void) {
  initialize(CONFIG_MAX_SENSOR_COUNT);
  ESP_LOGI(TAG, "Initialized sensor-configuration for %i",
           CONFIG_MAX_SENSOR_COUNT);
  add_sensor(10);
  add_sensor(11);
  sensor_reading reading;
  // calibrate_air(10);
  // calibrate_water(10);
  read_sensor_data(10, &reading);
}
