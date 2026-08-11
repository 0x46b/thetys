/*! @file
 * @todo This should belong to the driver, really...
 * Dynamic store for sensor-configurations. Hold the GPIO and calibration-infos.
 *
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

#ifndef SENSOR_CONFIGURATION_H
#define SENSOR_CONFIGURATION_H

#include "sensor_mgr_type_definitions.h"
#include <esp_log.h>
#include <stdint.h>

/*! Result-type to be able to determine the result of an operation */
typedef enum SENSOR_REGISTER_RESULT {
  SENSOR_CFG_OK = 0, /*!< Everything was fine, operation successfull*/
  SENSOR_CFG_ERROR =
      1, /*!< Something went wrong, but we do not know what exactly */
  SENSOR_CFG_LOW_MEMORY =
      2, /*!< Not enough memory for completing the operation */
  SENSOR_CFG_UNKNOWN_SENSOR_ID =
      4, /*!< The operation was given a sensor-id that is not registered */
  SENSOR_CFG_UNINITIALIZED = 5, /*!< Array that is given is not initialized */
  SENSOR_CFG_INITIALIZATION_ERROR =
      6, /*!< Dynamic array could not get initialized correctly */
  SENSOR_CFG_PARAMETER_NULL = 7, /*!< Parameter is null */
} SENSOR_REGISTER_RESULT;

/*! Calibration data used for calculating the humidity-percentage */
typedef struct sensor_calibration_data_T {
  bool calibrated; /*!< Determines, if the calibration was executed or if these
                      are default values */
  uint32_t air_measurement; /*!< The value that gots measured by calibrate_air
                          if the sensor is in the air, typically 4095 */
  uint32_t
      water_measurement;  /*!< The value that gots measure by calibrate_water if
                        the sensor is in a glass of water */
  uint32_t air_reference; /*!< Reference value, that we would expect for "no
                             water" */

  uint32_t
      water_reference; /*!< Reference value that we expect for "full water" */
} sensor_calibration_data_T;

/*! Configuration for a sensor */
typedef struct sensor_T {
  sensor_handle_T handle;
  uint32_t sensor_gpio; /*!< The GPIO where the sensor is attached*/
  sensor_calibration_data_T
      calibration_data; /*!< Calibration data for the sensor */
} sensor_T;

/*! Dynamic list to be able to attach/remove sensors dynamically */
typedef struct sensor_register_T {
  sensor_T
      *configurations; /*!< Dynamic array of all known sensor_configurations */
  uint32_t size;       /*!< Current size of the array (how much memory we have
                          allocated) */
  uint32_t used;       /*!< Number of used configurations */
} sensor_register_T;

/*! Initializes the dynamic-configuration array by allocating enough memory and
 * setting initial values
 * @param initialSize Initial size of the array
 * @return SENSOR_CFG_OK The array got successfully
 * initialized
 * @return SENSOR_CFG_LOW_MEMORY Not enough memory to allocate
 * enough space for initialSize-elements
 */
SENSOR_REGISTER_RESULT
sensor_register_initialize(size_t initialSize);

/*! Inserts a configuration into the dynamic-array
 * @param element The configuration that should get stored in the array
 * @return SENSOR_CFG_OK The configuration got stored
 * successfully
 * @return SENSOR_CFG_LOW_MEMORY Not enough memory to allocate
 * space needed
 * @return SENSOR_CFG_UNINITIALIZED Array is not initialized,
 * call init_configurations first
 */
SENSOR_REGISTER_RESULT
sensor_register_insert(sensor_T element, sensor_handle_T *handle);

/*! Resets the dynamic array, frees all memory for the stored configurations
 * @return SENSOR_CFG_OK The configuration got reset
 * successfully
 */
SENSOR_REGISTER_RESULT
sensor_register_free();

/*! Gets the configuration for a registered sensor with the id sensor_id
 * @param[out] sensor_config pointer to an already allocated sensor_config
 * @param[in] sensor_id Id of the sensor
 * @returns SENSOR_CONFIG_OK Everything had worked
 * @returns SENSOR_CFG_UNKNOWN_SENSOR_ID Sensor with the given id is not known
 * (NOTE: sensor-ids start with 0)
 */
SENSOR_REGISTER_RESULT
sensor_register_fetch(sensor_handle_T sensor_handle, sensor_T *sensor_config);

/*! Gets the count of currently registered sensors
 * @param[out] The number of registered sensors
 */
SENSOR_REGISTER_RESULT sensor_register_get_count(uint32_t *count);
#endif // SENSOR_CONFIGURATION_H
