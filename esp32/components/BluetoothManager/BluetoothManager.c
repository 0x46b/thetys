#include "BluetoothManager.h"
#include "common.h"
#include "gap.h"
#include "gatt_svc.h"
#include "plant_service.h"
#include "portmacro.h"

static new_plant_callback_t new_plant_callback;

/* Library function declarations */
void ble_store_config_init(void);

/* Private function declarations */
static void on_stack_reset(int reason);
static void on_stack_sync(void);
static void nimble_host_config_init(void);
static void nimble_host_task(void *param);

BLU_RESULT blue_subscribe_new_plant(new_plant_callback_t callback) {
  if (new_plant_callback != NULL) {
    ESP_LOGE(TAG, "Callback already subscribed!");
    return BLU_CALLBACK_ALREADY_SUBSCRIBED;
  }

  new_plant_callback = callback;

  return BLU_OK;
}

/* Private functions */
/*
 *  Stack event callback functions
 *      - on_stack_reset is called when host resets BLE stack due to errors
 *      - on_stack_sync is called when host has synced with controller
 */
static void on_stack_reset(int reason) {
  /* On reset, print reset reason to console */
  ESP_LOGI(TAG, "nimble stack reset, reset reason: %d", reason);
}

static void on_stack_sync(void) {
  /* On stack sync, do advertising initialization */
  advertising_init();
}

static void nimble_host_config_init(void) {
  /* Set host callbacks */
  ble_hs_cfg.reset_cb = on_stack_reset;
  ble_hs_cfg.sync_cb = on_stack_sync;
  ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;
  ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

  /* Store host configuration */
  ble_store_config_init();
}

static void nimble_host_task(void *param) {
  /* Task entry log */
  ESP_LOGI(TAG, "nimble host task has been started!");

  /* This function won't return until nimble_port_stop() is executed */
  nimble_port_run();

  /* Clean up at exit */
  vTaskDelete(NULL);
}

static void plant_data_task(void *param) {
  /* Task entry log */
  ESP_LOGI(TAG, "Plant data task has been started");

  /* Loop forever */
  while (1) {
    /* Update heart rate value every 1 second */
    update_humidities();
    ESP_LOGI(TAG, "Humidity data got updated");

    /* Send heart rate indication if enabled */
    // send_heart_rate_indication();

    /* Sleep */
    vTaskDelay(PLANT_DATA_UPDATE_PERIOD_MS / portTICK_PERIOD_MS);
  }

  /* Clean up at exit */
  vTaskDelete(NULL);
}

BLU_RESULT blu_initialize(void) {
  /* Local variables */
  BaseType_t rc = 0;
  esp_err_t ret;

  /*
   * NVS flash initialization
   * Dependency of BLE stack to store configurations
   */
  ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "failed to initialize nvs flash, error code: %d ", ret);
    return BLU_INITIALIZATION_ERROR;
  }

  /* NimBLE stack initialization */
  ret = nimble_port_init();
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "failed to initialize nimble stack, error code: %d ", ret);
    return BLU_INITIALIZATION_ERROR;
  }

  /* GAP service initialization */

  ble_svc_gap_device_name_set(BLUETOOTH_DEVICE_NAME);
  rc = gap_init();
  if (rc != 0) {
    ESP_LOGE(TAG, "failed to initialize GAP service, error code: %d", rc);
    return BLU_INITIALIZATION_ERROR;
  }

  /* GATT server initialization */
  rc = gatt_svc_init();
  if (rc != 0) {
    ESP_LOGE(TAG, "failed to initialize GATT server, error code: %d", rc);
    return BLU_INITIALIZATION_ERROR;
  }

  /* NimBLE host configuration initialization */
  nimble_host_config_init();

  /* Start NimBLE host task thread and return */
  rc = xTaskCreate(nimble_host_task, "NimBLE Host", 4 * 1024, NULL, 5, NULL);
  if (rc != pdPASS) {
    ESP_LOGE(TAG, "failed to create NimBLE host task");
    return BLU_TASK_CREATION_FAILED;
  }

  rc = xTaskCreate(plant_data_task, "Plant Data Updater", 4 * 1024, NULL, 5,
                   NULL);
  if (rc != pdPASS) {
    ESP_LOGE(TAG, "failed to create plant data updater task");
    return BLU_TASK_CREATION_FAILED;
  }
  return BLU_OK;
}
