#ifndef PLANT_REPOSITORY_H
#define PLANT_REPOSITORY_H

#include "plant_mgr_type_definitions.h"
#include <esp_err.h>

esp_err_t plant_repo_save_plants(plant_repository_T plan_repository);
esp_err_t plant_repo_load_plants(plant_repository_T *plant_repository);
esp_err_t plant_repo_insert(plant_T plant);
esp_err_t plant_repo_fetch(plant_handle_T handle, plant_T *plant);
esp_err_t plant_repo_update(plant_T plant);
esp_err_t plant_repo_initialize(void);

#endif // PLANT_REPOSITORY_H
