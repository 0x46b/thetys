// Should have a dynamic array to store and retrieve registered relais
#ifndef RELAIS_REGISTER_H
#define RELAIS_REGISTER_H

#include "relais_types.h"
#include <esp_err.h>
#include <stddef.h>
#include <stdint.h>

typedef struct relais_T {
  uint32_t relais_gpio;
  uint32_t relais_handle;
} relais_T;

typedef struct relais_register_T {
  relais_T *relais;
  uint32_t size;
  uint32_t used;
} relais_register_T;

esp_err_t relais_register_initialize(size_t initial_size);
esp_err_t relais_register_insert(relais_T relais);
esp_err_t relais_register_fetch(relais_handle_T handle, relais_T *relais);
esp_err_t relais_register_free();

#endif // RELAIS_REGISTER_H
