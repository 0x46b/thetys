#ifndef PLANT_SERVICE_H
#define PLANT_SERVICE_H

#include "plant_mgr_type_definitions.h"
#include <stddef.h>
#include <stdint.h>

typedef struct humidity_data_T {
  const char *plant_name;
  size_t plant_name_length;

  plant_handle_T plant_handle;
  uint32_t humidity_percentage;
  bool pump_running;
  bool is_active;
} humidity_data_T;

humidity_data_T *update_humidities(void);
char *jsonify_humidities(void);

#endif // PLANT_SERVICE_H
