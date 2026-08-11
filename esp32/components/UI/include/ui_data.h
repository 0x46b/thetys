#ifndef UI_DATA_H
#define UI_DATA_H

#include <esp_err.h>
#include <sdkconfig.h>
#include <stdint.h>
#include <time.h>

typedef struct measurement {
  uint32_t sensor_id;
  time_t timestamp;
  uint32_t humidity_in_percent;
} measurement;

typedef struct plant_state {
  char *plant_name;
  uint32_t sensor_id;

  bool pump_running;

  measurement measurements[CONFIG_NUMBER_OF_HISTORIC_MEASUREMENTS];
  measurement newest_measurement;

  time_t last_watering;
  uint32_t configured_watering_threshold;
} plant_state;

esp_err_t ui_add_plant_state_screen(plant_state state);
esp_err_t ui_add_measurement(measurement measurement);
#endif // UI_DATA_H
