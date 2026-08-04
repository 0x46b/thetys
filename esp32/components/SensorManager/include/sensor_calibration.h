#ifndef SENSOR_CALIBRATION_H
#define SENSOR_CALIBRATION_H
/*! @file
 * Routines for helping with the sensor-calibration
 */
#include "sensor_configuration.h"
#include <stdint.h>

/*! Result codes for calibration routines
 */
typedef enum CALIB_RESULT {
  CALIB_SUCCESS = 0, /*!< Everything worked */
  CALIB_LOWMEM = 1,  /*!< Not enough memory */
  CALIB_IOERROR = 2, /*!< IO-error while trying to read from the sensor  */
  CALIB_UNINITIALIZED_PARAM = 3 /*!< out-parameter was NULL */
} CALIB_RESULT;

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
CALIB_RESULT get_calibration_factor(sensor_configuration sensor_conf,
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
CALIB_RESULT get_calibration_samples(sensor_configuration sensor_conf,
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
CALIB_RESULT clean_samples(float *sample_array, uint32_t number_of_samples,
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
CALIB_RESULT get_sample_average(float *sample_array, uint32_t number_of_samples,
                                float *sample_average);

#endif // SENSOR_CALIBRATION_H
