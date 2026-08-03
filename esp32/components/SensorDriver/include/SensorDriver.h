#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H
/*! @file
 * Driver for the hardware-sensors
 */

#include <stdint.h>

float sensor_drv_read(uint32_t sensor_gpio);

#endif // SENSOR_DRIVER_H
