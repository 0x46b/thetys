#include "SensorDriver.h"
#include "config.h"
#include "hal/adc_types.h"
#include <driver/gpio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_err.h>
#include <esp_log.h>
#include <stdint.h>

static const char *TAG = "SensorDriver";
static adc_oneshot_unit_handle_t adc1_handle;

esp_err_t initialize_adc_driver() {
  adc_oneshot_unit_init_cfg_t init_config1 = {
      .unit_id = ADC_UNIT_1,
      .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));
  ESP_LOGI(TAG, "ADC driver initialized");
  return ESP_OK;
}

esp_err_t initialize_channel(adc_channel_t sensor_channel) {
  adc_oneshot_chan_cfg_t config = {
      .bitwidth = ADC_BITWIDTH_DEFAULT,
      .atten = get_sdkconfig_attentuation(),
  };
  ESP_ERROR_CHECK(
      adc_oneshot_config_channel(adc1_handle, sensor_channel, &config));

  ESP_LOGI(TAG, "ADC Channel configured");
  return ESP_OK;
}

SENSOR_RESULT sensor_drv_initialize(uint32_t sensor_gpio) {
  adc_unit_t sensor_unit_id;
  adc_channel_t sensor_channel;
  ESP_LOGI(TAG, "Trying to start ADC on GPIO %i", sensor_gpio);
  ESP_ERROR_CHECK(
      adc_oneshot_io_to_channel(sensor_gpio, &sensor_unit_id, &sensor_channel));

  if (sensor_unit_id == ADC_UNIT_2) {
    ESP_LOGE(TAG, "Only ADC 1 is allowed (GPIO 32..39)");
    return SENSOR_INVALID_GPIO;
  }

  // First call, so initialize the driver
  if (adc1_handle == NULL) {
    initialize_adc_driver();
  }

  initialize_channel(sensor_channel);
  ESP_LOGI(TAG, "ADC for GPIO %i initialized", sensor_gpio);
  return SENSOR_SUCCESS;
}

SENSOR_RESULT sensor_drv_read(uint32_t sensor_gpio, uint32_t *sensor_value) {
  adc_unit_t sensor_unit_id;
  adc_channel_t sensor_channel;

  if (sensor_value == NULL) {
    ESP_LOGE(TAG, "sensor_value is NULL");
    return SENSOR_ERROR;
  }

  if (adc1_handle == NULL) {
    ESP_LOGE(TAG,
             "ADC handle is null. Please call sensor_drv_initialize() first");
    return SENSOR_ERROR;
  }

  ESP_ERROR_CHECK(
      adc_oneshot_io_to_channel(sensor_gpio, &sensor_unit_id, &sensor_channel));
  ESP_LOGD(TAG, "Resolved GPIO %i to Channel %i", sensor_gpio, sensor_channel);

  if (sensor_unit_id == ADC_UNIT_2) {
    ESP_LOGE(TAG, "Only ADC 1 is allowed (GPIO 32..39)");
    return SENSOR_INVALID_GPIO;
  }

  int raw_value;
  ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, sensor_channel, &raw_value));
  ESP_LOGD(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1,
           sensor_channel, raw_value);

  *sensor_value = raw_value;

  return SENSOR_SUCCESS;
}
