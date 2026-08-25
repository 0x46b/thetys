/*! @file
 * Task for fetching the data from a sensor-Queue and saving it into a
 * repository for future use
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
#ifndef SENSOR_DATA_TASK_H
#define SENSOR_DATA_TASK_H

#include "plant_mgr_type_definitions.h"
#include <esp_err.h>

esp_err_t sensor_data_saving_task_create(plant_T *plant);

#endif // SENSOR_DATA_TASK_H
