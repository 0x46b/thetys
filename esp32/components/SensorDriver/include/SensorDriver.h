/*! @file
 * Driver for the hardware-sensors
 * Copyright (C) 2026 Sebastian Murschall <sebastian.murschall@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <stdint.h>

typedef enum SENSOR_RESULT {
  SENSOR_SUCCESS = 0,
  SENSOR_INVALID_GPIO = 2,
  SENSOR_ERROR = 3
} SENSOR_RESULT;

SENSOR_RESULT sensor_drv_initialize(uint32_t sensor_gpio);
SENSOR_RESULT sensor_drv_read(uint32_t sensor_gpio, uint32_t *sensor_value);

#endif // SENSOR_DRIVER_H
