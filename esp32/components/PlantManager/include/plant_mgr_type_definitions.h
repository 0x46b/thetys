#ifndef PLANT_DATA_H
#define PLANT_DATA_H

#include "freertos/idf_additions.h"
#include "relais_types.h"
#include "sensor_mgr_type_definitions.h"

#define MAX_PLANTS 10
typedef uint32_t plant_handle_T;

typedef struct plant_T {
  const char *plant_name;
  plant_handle_T handle;
  uint32_t watering_threshold;
  relais_handle_T pump_handle;
  sensor_handle_T sensor_handle;
  uint32_t pumping_time_ms;
  QueueHandle_t data_queue_handle;
  TaskHandle_t reading_task_handle;
  TaskHandle_t data_task_handle;
  bool is_active; /*!< Set to false to disable measurements and automatic
                     watering */
} plant_T;

typedef struct plant_repository_T {
  plant_T *plants;
  uint32_t used;
  uint32_t size;
} plant_repository_T;

typedef enum pump_state_T {
  PUMP_OFF = 0,
  PUMP_ON = 1,
  PUMP_TIMED = 2
} pump_state_T;

#endif // PLANT_DATA_H
