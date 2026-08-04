#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H
/*! @file
 * Driver for the hardware-sensors
 */

#include <stdint.h>

typedef enum SENSOR_RESULT {
  SENSOR_SUCCESS = 0,
  SENSOR_INVALID_GPIO = 2,
  SENSOR_ERROR = 3
} SENSOR_RESULT;

SENSOR_RESULT sensor_drv_initialize(uint32_t sensor_gpio);
SENSOR_RESULT sensor_drv_read(uint32_t sensor_gpio, uint32_t *sensor_value);

#endif // SENSOR_DRIVER_H
