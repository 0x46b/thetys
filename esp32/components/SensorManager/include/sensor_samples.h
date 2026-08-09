/*! @file
 * Dynamic store for sensor-data.
 * @todo replace custom return codes with ESP-defaults.
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

#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

// Force ESP32 architecture definitions to settle first
#include <esp_err.h>
#include <stdint.h>

typedef struct sensor_sample_array {
  uint32_t *samples;
  uint32_t size;
  uint32_t used;
} sensor_sample_array;

esp_err_t get_data_for_sensor(uint32_t sensor_id, uint32_t *sample);
esp_err_t sensor_data_initialize(uint32_t size);
esp_err_t sensor_data_insert(uint32_t sample);
esp_err_t sensor_data_update(uint32_t id, uint32_t sample);
esp_err_t sensor_data_reset();

#endif // SENSOR_DATA_H
