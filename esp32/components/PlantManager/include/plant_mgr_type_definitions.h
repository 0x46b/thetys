#ifndef PLANT_DATA_H
#define PLANT_DATA_H

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
  bool is_active; /*!< Set to false to disable measurements and automatic
                     watering */
} plant_T;

typedef struct plant_repository_T {
  plant_T plants[MAX_PLANTS];
  uint32_t used;
} plant_repository_T;
#endif // PLANT_DATA_H
