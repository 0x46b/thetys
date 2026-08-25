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
#include "relais_register.h"
#include "esp_err.h"
#include "esp_log.h"
#include "relais_types.h"

static relais_register_T relais_register;
static const char *TAG = "relais_register";

/* Private methods */
bool relais_register_handle_exists(relais_handle_T handle) {
  return handle < relais_register.used;
}

/* Public methods */
esp_err_t relais_register_initialize(size_t initial_size) {
  if (initial_size == 0) {
    ESP_LOGE(TAG, "Initializing with 0 elements is not allowed.");
    return ESP_ERR_INVALID_SIZE;
  }

  ESP_LOGD(TAG, "Initializing array for %i relais.", initial_size);
  relais_register.relais = malloc(initial_size * sizeof(relais_T));

  if (relais_register.relais == NULL) {
    ESP_LOGE(TAG, "Malloc failed!");
    return ESP_ERR_NO_MEM;
  }

  relais_register.used = 0;
  relais_register.size = initial_size;

  ESP_LOGD(TAG, "Successfully initialized memory for %i relais.", initial_size);
  return ESP_OK;
}

esp_err_t relais_register_insert(relais_T relais, relais_handle_T *handle) {
  if (relais_register.relais == NULL) {
    ESP_LOGE(TAG, "Try to insert into uninitialized array");
    return ESP_ERR_NOT_ALLOWED;
  }

  if (relais_register.used == relais_register.size) {
    uint32_t new_size = relais_register.size * 2;

    ESP_LOGD(TAG, "Not enough space for added relais, resizing from %i to %i",
             relais_register.size, new_size);
    relais_T *new_array =
        realloc(relais_register.relais, new_size * sizeof(relais_T));
    if (new_array == NULL) {
      ESP_LOGE(TAG, "Resizing failed");
      return ESP_ERR_NO_MEM;
    }
    relais_register.size = new_size;
    relais_register.relais = new_array;
  }

  uint32_t new_index = relais_register.used;
  *handle = new_index;
  relais.handle = new_index;
  relais_register.relais[new_index] = relais;

  ESP_LOGD(TAG, "Inserted new relais [Handle: %i](%i used total)", *handle,
           relais.handle, relais_register.used);

  return ESP_OK;
}

esp_err_t relais_register_fetch(relais_handle_T handle, relais_T *relais) {
  if (!relais_register_handle_exists(handle)) {
    ESP_LOGE(TAG, "No plant with handle %i found", handle);
    return ESP_ERR_NOT_FOUND;
  }

  *relais = relais_register.relais[(uint32_t)handle];

  return ESP_OK;
}

esp_err_t relais_register_free() {
  free(relais_register.relais);
  relais_register.relais = NULL;
  relais_register.used = 0;
  relais_register.size = 0;

  ESP_LOGD(TAG, "Reset registered relais");
  return ESP_OK;
}
