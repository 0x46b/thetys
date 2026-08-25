/*! @file
 * Routines for helping with the sensor-calibration
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
#ifndef SENSOR_CALIBRATION_H
#define SENSOR_CALIBRATION_H

#include "esp_err.h"
#include "sensor_mgr_type_definitions.h"
#include "sensor_register.h"
#include <stdint.h>

/*! Reads number_of_samples samples from the sensor with sensor_id, cleans the
 * readings and returns the average in calibration_value
 * @param[in] sensor_conf Configuration of the sensor to use
 * @param[in] number_of_samples Number of samples for averaging over
 * @param[out] calibration_factor the calculated calibration-factor. Make sure
it points to valid memory.
 * @return #CALIB_SUCCESS No errors, everything fine.
 * @return #CALIB_LOWMEM Memoryallocation for the sample-table has
failed
 * @return  #CALIB_IOERROR Reading from the sensor failed due
hardware-issues
 * @return #CALIB_UNINITIALIZED_PARAM calibration_factor is null (I
told you so!)
 */
esp_err_t get_calibration_factor(sensor_T sensor_conf,
                                 uint32_t number_of_samples,
                                 float *calibration_factor);

/*! Reads number_of_samples from the sensor with sensor_id and returns it in
sample_array
* @param[in] sensor_conf Configuration of the sensor to use
* @param[in] number_of_samples Number of samples to take
* @param[out] sample_array Array containing all samples (don't forget to free
it)
* @return #CALIB_SUCCESS Everything went fine
* @return #CALIB_LOWMEM Not enough memory to allocate for all
samples
*/
esp_err_t get_calibration_samples(sensor_T sensor_config,
                                  uint32_t number_of_samples,
                                  float *sample_array);

/*! Takes an array of samples and cleans it from possible runaways
 *
 * It does it by calculating the average, the variance and the
 * standard-deviation and filtering out every sample that deviates more than
 * double the standard-deviation
 * @param[in] sample_array Array with all samples we want to clean
 * @param[in] number_of_samples Number of samples (or "size" of the given array)
 * @param[out] cleaned_samples New array without runaway values
 * @param[out] number_of_cleaned_samples Number of samples in the "clean" set
 * @return #CALIB_SUCCESS Everything went fine
 * @return #CALIB_LOWMEM Not enough memory to allocate for all
 * samples
 */
esp_err_t clean_samples(float *sample_array, uint32_t number_of_samples,
                        float *cleaned_samples,
                        uint32_t *number_of_cleaned_samples);

/*! Takes an array of samples and calculates the average
 * @param[in] sample_array Array with all samples
 * @param[in] number_of_samples Number of samples (or "size" of the given array)
 * @param[out] sample_average Average over all samples (has to point to valid
 * memory)
 * @return #CALIB_SUCCESS Everything went fine
 * @return #CALIB_LOWMEM Not enough memory to allocate for all
 * samples
 */
esp_err_t get_sample_average(float *sample_array, uint32_t number_of_samples,
                             float *sample_average);

uint32_t sensor_get_humidity(int32_t raw_value,
                             sensor_calibration_data_T calibration);
#endif // SENSOR_CALIBRATION_H
