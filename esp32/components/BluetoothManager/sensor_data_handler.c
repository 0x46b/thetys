#include "sensor_data_handler.h"
#include <SensorManager.h>
#include <esp_log.h>
#include <stdint.h>

static const char *TAG = "sensor_data_svc";

uint32_t get_sensor_data(uint32_t sensor_id) {
  /* sensor_reading_t reading; */
  /* SMGR_RESULT result; */
  /* result = sensor_manager_read_sensor(sensor_id, &reading); */
  /* if (result != SMGR_SUCCESS) { */
  /*   ESP_LOGE(TAG, "Could not read sensor data"); */
  /*   return 0; */
  /* } */

  /* return reading.humidity_percentage; */
  return 30;
}
