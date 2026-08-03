#ifndef SENSOR_CONFIGURATION_H
#define SENSOR_CONFIGURATION_H

/*! @file
Methods for handling a dynamic-array of sensor-configurations
*/
#include <esp_log.h>
#include <stdint.h>

static const char *CONF_TAG = "sensor_configuration";

/*! Result-type to be able to determine the result of an operation */
typedef enum SENSOR_CONFIGURATION_RESULT {
  SUCCESS = 0, /*!< Everything was fine, operation successfull*/
  UNKNOWN_ERROR =
      1,          /*!< Something went wrong, but we do not know what exactly */
  LOW_MEMORY = 2, /*!< Not enough memory for completing the operation */
  IO_ERROR = 3,   /*!< IO-error while reading from the sensor */
  UNKNOWN_SENSOR_ID =
      4, /*!< The operation was given a sensor-id that is not registered */
  UNINITIALIZED = 5, /*!< Array that is given is not initialized */
} SENSOR_CONFIGURATION_RESULT;

/*! Calibration data used for calculating the humidity-percentage */
typedef struct sensor_calibration_data {
  bool calibrated; /*!< Determines, if the calibration was executed or if these
                      are default values */
  uint32_t air_factor; /*!< The value that gots measured by calibrate_air if the
                          sensor is in the air */
  uint32_t water_factor; /*!< The value that gots measure by calibrate_water if
                            the sensor is in a glass of water */
} sensor_calibration_data;

/*! Configuration for a sensor */
typedef struct sensor_configuration {
  uint32_t sensor_id;   /*!< The sensor-id */
  uint32_t sensor_gpio; /*!< The GPIO where the sensor is attached*/
  sensor_calibration_data
      calibration_data; /*!< Calibration data for the sensor */
} sensor_configuration;

/*! Dynamic list to be able to attach/remove sensors dynamically */
typedef struct sensor_configuration_data {
  sensor_configuration
      *configurations; /*!< Dynamic array of all known sensor_configurations */
  uint32_t last_id;    /*!< Last given id, will be used by add_sensor to
                          automatically set an unused id */
  uint32_t size;       /*!< Current size of the array (how much memory we have
                          allocated) */
  uint32_t used;       /*!< Number of used configurations */
} sensor_configuration_data;

/*! Initializes the dynamic-configuration array by allocating enough memory and
 * setting initial values
 * @param configs Pointer to the array that should get initialized
 * @param initialSize Initial size of the array
 * @return SENSOR_CONFIGURATION_RESULT::SUCCESS The array got successfully
 * initialized
 * @return SENSOR_CONFIGURATION_RESULT::LOW_MEMORY Not enough memory to allocate
 * enough space for initialSize-elements
 */
SENSOR_CONFIGURATION_RESULT
init_configurations(sensor_configuration_data *configs, size_t initialSize);

/*! Inserts a configuration into the dynamic-array
 * @param configs Pointer to the array in which the item should get stored into
 * @param element The configuration that should get stored in the array
 * @return SENSOR_CONFIGURATION_RESULT::SUCCESS The configuration got stored
 * successfully
 * @return SENSOR_CONFIGURATION_RESULT::LOW_MEMORY Not enough memory to allocate
 * space needed
 * @return SENSOR_CONFIGURATION_RESULT::UNINITIALIZED Array is not initialized,
 * call init_configurations first
 */
SENSOR_CONFIGURATION_RESULT
insert_configuration(sensor_configuration_data *configs,
                     sensor_configuration element);

/*! Resets the dynamic array, frees all memory for the stored configurations
 * @param configs Pointer to the array in which the item should get stored into
 * @return SENSOR_CONFIGURATION_RESULT::SUCCESS The configuration got reset
 * successfully
 */
SENSOR_CONFIGURATION_RESULT
free_configurations(sensor_configuration_data *configs);
#endif // SENSOR_CONFIGURATION_H
