#include "sensor_calibration.h"
#include <SensorDriver.h>
#include <esp_log.h>
#include <math.h>
#include <sdkconfig.h>

CALIB_RESULT clean_samples(float *sample_array, uint32_t number_of_samples,
                           float *cleaned_samples,
                           uint32_t *number_of_cleaned_samples) {
  if (sample_array == NULL) {
    ESP_LOGE(CALIB_TAG, "sample_array is NULL");
    return CALIB_UNINITIALIZED_PARAM;
  }

  if (number_of_cleaned_samples == NULL) {
    ESP_LOGE(CALIB_TAG, "number_of_cleaned_samples is NULL");
    return CALIB_UNINITIALIZED_PARAM;
  }

  if (number_of_samples <= 0) {
    // Do something, I dunno?
  }

  double variance = 0.0;
  double standard_deviation = 0.0;
  float average = 0.0;
  CALIB_RESULT result =
      get_sample_average(sample_array, number_of_samples, &average);

  if (result != CALIB_SUCCESS) {
    ESP_LOGE(CALIB_TAG, "Something went very wrong here?");
    return result;
  }

  for (int i = 0; i < number_of_samples; i++) {
    variance += pow(sample_array[i] - average, 2);
  }

  standard_deviation = sqrt(variance / number_of_samples);

  float *buffer = malloc(number_of_samples * sizeof(float));
  if (buffer == NULL) {
    ESP_LOGE(CALIB_TAG,
             "Could not allocate enough memory for the return-values");
    return CALIB_LOWMEM;
  }

  int target_index = 0;
  for (int i = 0; i < number_of_samples; i++) {
    double difference = fabs(sample_array[i] - average);

    if (difference <= (2.0 * standard_deviation)) {
      buffer[target_index] = sample_array[i];
      target_index++;
    }
  }

  *number_of_cleaned_samples = target_index + 1;
  cleaned_samples = malloc(*number_of_cleaned_samples * sizeof(float));
  memcpy(cleaned_samples, buffer, *number_of_cleaned_samples * sizeof(float));

  free(buffer);
  ESP_LOGI(CALIB_TAG, "Removed %i samples (%i clean samples total)",
           (number_of_samples - *number_of_cleaned_samples),
           *number_of_cleaned_samples);
  return CALIB_SUCCESS;
}

CALIB_RESULT get_sample_average(float *sample_array, uint32_t number_of_samples,
                                float *sample_average) {
  if (sample_array == NULL) {
    ESP_LOGE(CALIB_TAG, "sample_array is NULL");
    return CALIB_UNINITIALIZED_PARAM;
  }

  if (sample_average == NULL) {
    ESP_LOGE(CALIB_TAG, "sample_average is NULL");
    return CALIB_UNINITIALIZED_PARAM;
  }

  double sum = 0.0;
  for (int i = 0; i < number_of_samples; i++) {
    sum += sample_array[i];
  }

  *sample_average = sum / number_of_samples;
  return CALIB_SUCCESS;
}

CALIB_RESULT get_calibration_factor(sensor_configuration sensor_conf,
                                    uint32_t number_of_samples,
                                    float *calibration_factor) {
  float sensor_reading_sum = 0;
  float current_sensor_reading = 0;
  float *sensor_samples = malloc(number_of_samples * sizeof(float));

  if (sensor_samples == NULL) {
    ESP_LOGE(CALIB_TAG,
             "Couldn't allocate memory for reading %i samples for calibration "
             "of sensor %i",
             number_of_samples, sensor_conf.sensor_id);
    return CALIB_LOWMEM;
  }

  if (calibration_factor == NULL) {
    ESP_LOGD(CALIB_TAG, "Parameter calibration_factor is NULL");
    return CALIB_UNINITIALIZED_PARAM;
  }

  for (uint32_t sample_no = 0; sample_no < CONFIG_NUM_CALIBRATION_SAMPLES;
       sample_no++) {
    current_sensor_reading = sensor_drv_read(sensor_conf.sensor_gpio);
    sensor_samples[sample_no] = current_sensor_reading;
    sensor_reading_sum += current_sensor_reading;
  }

  *calibration_factor = sensor_reading_sum / number_of_samples;

  free(sensor_samples);
  return CALIB_SUCCESS;
}
