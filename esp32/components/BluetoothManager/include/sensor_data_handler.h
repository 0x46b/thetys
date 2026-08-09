#ifndef SENSOR_DATA_HANDLER_H
#define SENSOR_DATA_HANDLER_H
#include <esp_random.h>

uint32_t get_sensor_data(uint32_t sensor_id);
uint32_t get_sensor_ids(void);

#endif // SENSOR_DATA_HANDLER_H
