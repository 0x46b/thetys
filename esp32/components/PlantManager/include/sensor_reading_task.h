/*! @file
 * Task for reading a sensor and putting the values into an freeRTOS-Queue
 */
/* Copyright (C) 2026 Sebastian Murschall <sebastian.murschall@gmail.com>
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
#ifndef SENSOR_READING_TASK_H
#define SENSOR_READING_TASK_H

#include "plant_mgr_type_definitions.h"
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <time.h>

typedef struct sensor_entry_T {
  plant_handle_T plant;
  uint32_t humidity_percentage;
  time_t timestamp;
} sensor_entry_T;

esp_err_t sensor_reading_task_create(plant_T *plant);

#endif // SENSOR_READING_TASK_H
