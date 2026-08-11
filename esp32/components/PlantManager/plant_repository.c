#include "plant_mgr_type_definitions.h"
#include <esp_err.h>
#include <esp_log.h>
#include <nvs.h>
#include <nvs_flash.h>

#define STORAGE_NAMESPACE "plant_data"
#define PLANT_KEY "plants"

static const char *TAG = "sensor_repository";

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
  ESP_LOGI(TAG, "Saving plants as blob...");
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

  ESP_LOGI(TAG, "Reading plant data blob:");
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
