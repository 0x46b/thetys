/*! @file
 * Methods to help with calibration of capacitive soil humitdity sensors
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
#include "sensor_mgr_type_definitions.h"
#include "sensor_register.h"
#include <SensorDriver.h>
#include <esp_log.h>
#include <math.h>
#include <sdkconfig.h>
#include <stdint.h>

static const char *TAG = "Calibration";

CALIB_RESULT clean_samples(float *sample_array, uint32_t number_of_samples,
                           float *cleaned_samples,
                           uint32_t *number_of_cleaned_samples) {
  if (sample_array == NULL) {
    ESP_LOGE(TAG, "sample_array is NULL");
    return CALIB_UNINITIALIZED_PARAM;
  }

  if (number_of_cleaned_samples == NULL) {
    ESP_LOGE(TAG, "number_of_cleaned_samples is NULL");
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
    ESP_LOGE(TAG, "Something went very wrong here?");
    return result;
  }

  for (int i = 0; i < number_of_samples; i++) {
    variance += pow(sample_array[i] - average, 2);
  }

  standard_deviation = sqrt(variance / number_of_samples);

  float *buffer = malloc(number_of_samples * sizeof(float));
  if (buffer == NULL) {
    ESP_LOGE(TAG, "Could not allocate enough memory for the return-values");
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
  ESP_LOGI(TAG, "Removed %i samples (%i clean samples total)",
           (number_of_samples - *number_of_cleaned_samples),
           *number_of_cleaned_samples);
  return CALIB_SUCCESS;
}

CALIB_RESULT get_sample_average(float *sample_array, uint32_t number_of_samples,
                                float *sample_average) {
  if (sample_array == NULL) {
    ESP_LOGE(TAG, "sample_array is NULL");
    return CALIB_UNINITIALIZED_PARAM;
  }

  if (sample_average == NULL) {
    ESP_LOGE(TAG, "sample_average is NULL");
    return CALIB_UNINITIALIZED_PARAM;
  }

  double sum = 0.0;
  for (int i = 0; i < number_of_samples; i++) {
    sum += sample_array[i];
  }

  *sample_average = sum / number_of_samples;
  return CALIB_SUCCESS;
}

CALIB_RESULT get_calibration_factor(sensor_T sensor_conf,
                                    uint32_t number_of_samples,
                                    float *calibration_factor) {
  float sensor_reading_sum = 0;
  uint32_t current_sensor_reading = 0;
  float *sensor_samples = malloc(number_of_samples * sizeof(float));

  if (sensor_samples == NULL) {
    ESP_LOGE(TAG,
             "Couldn't allocate memory for reading %i samples for calibration "
             "of sensor %i",
             number_of_samples, sensor_conf.sensor_gpio);
    return CALIB_LOWMEM;
  }

  if (calibration_factor == NULL) {
    ESP_LOGD(TAG, "Parameter calibration_factor is NULL");
    return CALIB_UNINITIALIZED_PARAM;
  }

  SENSOR_RESULT result;

  for (uint32_t sample_no = 0; sample_no < CONFIG_NUM_CALIBRATION_SAMPLES;
       sample_no++) {
    result = sensor_drv_read(sensor_conf.sensor_gpio, &current_sensor_reading);
    if (result != SENSOR_SUCCESS) {
      ESP_LOGE(TAG, "Error while reading sample");
      abort();
    }
    sensor_samples[sample_no] = current_sensor_reading;
    sensor_reading_sum += current_sensor_reading;
  }

  *calibration_factor = sensor_reading_sum / number_of_samples;

  free(sensor_samples);
  return CALIB_SUCCESS;
}

uint32_t get_humidity(int32_t raw_value,
                      sensor_calibration_data_T calibration) {
  ESP_LOGI(TAG, "Calculating humidity-percentage (adc: %i, air: %i, water: %i)",
           raw_value, calibration.air_measurement,
           calibration.water_measurement);

  float humidity_float =
      (float)(raw_value - calibration.air_measurement) * 100.0f /
      (float)(calibration.water_measurement - calibration.air_measurement);
  float rounded_float = nearbyintf(humidity_float);

  if (rounded_float < 0.0f) {
    rounded_float = 0.0f;
  } else if (rounded_float > 100.0f) {
    rounded_float = 100.0f;
  }

  return (uint32_t)rounded_float;
}
