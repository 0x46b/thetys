#include "plant_mgr_type_definitions.h"
#include <esp_err.h>
#include <esp_log.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <stddef.h>

#define STORAGE_NAMESPACE "plant_data"
#define PLANT_KEY "plants"

static const char *TAG = "PlantManager.plant_repository";
static plant_repository_T plant_repository;

/* Private methods */
esp_err_t plant_repo_initialize_memory(size_t initial_size) {
  if (initial_size == 0) {
    ESP_LOGE(TAG, "Initializing with 0 elements is not allowed.");
    return ESP_FAIL;
  }

  ESP_LOGD(TAG, "Initializing array for %i plants.", initial_size);
  plant_repository.plants = malloc(initial_size * sizeof(plant_T));

  if (plant_repository.plants == NULL) {
    ESP_LOGE(TAG, "Malloc failed!");
    return ESP_ERR_NO_MEM;
  }

  plant_repository.used = 0;
  plant_repository.size = initial_size;

  ESP_LOGD(TAG, "Successfully initialized memory for %i plants.", initial_size);
  return ESP_OK;
}

bool plant_repo_handle_exists(plant_handle_T handle) {
  return handle < plant_repository.used;
}

/* Public methods */
esp_err_t plant_repo_insert(plant_T plant, plant_handle_T *handle) {
  if (plant_repository.plants == NULL) {
    ESP_LOGE(TAG, "Try to insert into uninitialized array");
    return ESP_ERR_NOT_ALLOWED;
  }

  if (plant_repository.used == plant_repository.size) {
    uint32_t new_size = plant_repository.size * 2;

    ESP_LOGD(TAG,
             "Not enough space for added sensor_config, resizing from %i to %i",
             plant_repository.size, new_size);
    plant_T *new_array =
        realloc(plant_repository.plants, new_size * sizeof(plant_T));
    if (new_array == NULL) {
      ESP_LOGE(TAG, "Resizing failed");
      return ESP_ERR_NO_MEM;
    }
    plant_repository.size = new_size;
    plant_repository.plants = new_array;
  }

  uint32_t new_index = plant_repository.used;
  *handle = new_index;
  plant.handle = new_index;
  plant_repository.plants[new_index] = plant;

  ESP_LOGD(TAG,
           "Inserted new plant [Handle: %i, sensor handle: %i, pump handle: "
           "%i, Threshold: %i](%i used total)",
           *handle, plant.sensor_handle, plant.pump_handle,
           plant.watering_threshold, plant_repository.used);

  return ESP_OK;
}

esp_err_t plant_repo_fetch(plant_handle_T handle, plant_T *plant) {
  if (!plant_repo_handle_exists(handle)) {
    ESP_LOGE(TAG, "No plant with handle %i found", handle);
    return ESP_ERR_NOT_FOUND;
  }

  *plant = plant_repository.plants[(uint32_t)handle];

  return ESP_OK;
}

esp_err_t plant_repo_update(plant_T plant) {
  if (!plant_repo_handle_exists(plant.handle)) {
    ESP_LOGE(TAG, "No plant with handle %i found", plant.handle);
    return ESP_ERR_NOT_FOUND;
  }

  plant_repository.plants[(uint32_t)plant.handle] = plant;

  return ESP_OK;
}

esp_err_t plant_repo_save_plants(plant_repository_T plan_repository) {
  nvs_handle_t my_handle;
  esp_err_t err;

  // Open NVS handle
  err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
    return err;
  }

  // Write blob
  ESP_LOGD(TAG, "Saving plants as blob...");
  err = nvs_set_blob(my_handle, PLANT_KEY, &plan_repository,
                     sizeof(plant_repository_T));
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to write plants blob!");
    nvs_close(my_handle);
    return err;
  }

  // Commit
  err = nvs_commit(my_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to commit data");
  }

  nvs_close(my_handle);
  return err;
}

esp_err_t plant_repo_load_plants(plant_repository_T *plant_repository) {
  nvs_handle_t my_handle;
  esp_err_t err;

  err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &my_handle);
  if (err != ESP_OK)
    return err;

  ESP_LOGD(TAG, "Reading plant data blob:");
  plant_repository_T loaded_repository;
  size_t data_size = sizeof(plant_repository_T);
  err = nvs_get_blob(my_handle, PLANT_KEY, &loaded_repository, &data_size);
  if (err == ESP_OK) {
    *plant_repository = loaded_repository;
  } else if (err == ESP_ERR_NVS_NOT_FOUND) {
    ESP_LOGW(TAG, "Test data not found!");
  }

  nvs_close(my_handle);
  return ESP_OK;
}

esp_err_t plant_repo_initialize(void) {
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  return ESP_OK;
}

esp_err_t plant_repo_free(void) {
  free(plant_repository.plants);
  plant_repository.plants = NULL;
  plant_repository.used = 0;
  plant_repository.size = 0;

  ESP_LOGD(TAG, "Reset plants");
  return ESP_OK;
}
