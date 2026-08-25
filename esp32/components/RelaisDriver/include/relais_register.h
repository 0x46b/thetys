/*! @file
 * Dynamic array for storing registered relais
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
#ifndef RELAIS_REGISTER_H
#define RELAIS_REGISTER_H

#include "relais_types.h"
#include <esp_err.h>
#include <stddef.h>
#include <stdint.h>

typedef struct relais_T {
  uint32_t relais_gpio;
  relais_handle_T handle;
} relais_T;

typedef struct relais_register_T {
  relais_T *relais;
  uint32_t size;
  uint32_t used;
} relais_register_T;

typedef enum fetch_result_T {
  FETCH_OK = 0,
  FETCH_FAILED = 1,
  FETCH_FINISHED = 2
} fetch_result_T;

esp_err_t relais_register_initialize(size_t initial_size);
esp_err_t relais_register_insert(relais_T relais, relais_handle_T *handle);
esp_err_t relais_register_fetch(relais_handle_T handle, relais_T *relais);
esp_err_t relais_register_reset_fetch(void);
fetch_result_T relais_register_fetch_next(relais_T *relais);
esp_err_t relais_register_free();

#endif // RELAIS_REGISTER_H
