/*! @file
 * API for managing sensors.
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

#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include "sensor_configuration.h"
#include <stdint.h>

/*! Operation result codes */
typedef enum SMGR_RESULT {
  SMGR_SUCCESS = 0,  /*!< Everything went as expected */
  SMGR_IO_ERROR = 1, /*!< The sensor is not readably due hardware-failure */
  SMGR_UNKNOWN_SENSOR =
      2, /*!< The sensor with the given id is not registered for handling */
  SMGR_MISSING_CALIBRATION = 3, /*!< Calibration-data for sensor is missing */
  SMGR_CATASTROPHIC_FAILURE =
      4, /*!< Something went terribly wrong, so we could not recover from it */
  SMGR_UNKNOWN_ERROR = 255 /*!< Something went wrong, but we dunno what */
} SMGR_RESULT;

/*! Struct for holding a sensor-reading */
typedef struct sensor_reading {
  uint32_t sensor_id;        /*!< The id of the corresponding sensor */
  float humidity_percentage; /*!< The humidity in percentage (0..1) */
} sensor_reading;

/*! Callback definition for a new data callback. Subscribe via
 * sensor_mgr_subscribe_new_measurement */
typedef void (*new_measurement_callback)(uint32_t, uint32_t);

/*! Callback definition for a new sensor callback. Subscribe via
 * sensor_mgr_subscribe_new_sensor */
typedef void (*new_sensor_callback)(sensor_configuration);

/*! Initializes the sensor-manager.
 * Sets the correct data-directions, allocates memory etc.
 * @return SMGR_RESULT::SMGR_SUCCESS Configurations successfully initialized
 * @return SMGR_RESULT::SMGR_CATASTROPHIC_FAILURE Something went terribly wrong
 */
SMGR_RESULT sensor_mgr_initialize(uint32_t number_of_sensors);

/*! Adds a sensor to the sensor-manager for monitoring
 * @param sensor_pin The GPIO where the sensor is attached to
 * @return SMGR_RESULT::SMGR_SUCCESS Sensor added successfully
 * @return SMGR_RESULT::SMGR_CATASTROPHIC_FAILURE Something went terribly wrong
 */
SMGR_RESULT sensor_mgr_add_sensor(uint32_t sensor_pin);

/*! Starts calibration for air-measurement
 * To calibrate the sensor we need to measure different "environments" to be
 * able to correct noise and stuff like this while calculating the humidity.
 * This calibration is meant for no water at all (= "sensor in the air")
 * @param sensor_id The id of the sensor we want to calibrate
 * @return SMGR_RESULT::SMGR_SUCCESS Calibration was successfull
 * @return SMGR_RESULT::SMGR_UNKNOWN_SENSOR No sensor with sensor_id was found
 * @return SMGR_RESULT::SMGR_IO_ERROR We could not read from the sensor
 * hardware-wise
 */
SMGR_RESULT calibrate_air(uint32_t sensor_id);

/*! Starts calibration for water-measurement
 * To calibrate the sensor we need to measure different "environments" to be
 * able to correct noise and stuff like this while calculating the humidity.
 * This calibration is meant for full water (= "sensor submerged in a glass of
 * water")
 * @param sensor_id The id of the sensor we want to calibrate
 * @return SMGR_RESULT::SMGR_SUCCESS Calibration was successfull
 * @return SMGR_RESULT::SMGR_UNKNOWN_SENSOR No sensor with sensor_id was found
 * @return SMGR_RESULT::SMGR_IO_ERROR We could not read from the sensor
 * hardware-wise
 */
SMGR_RESULT calibrate_water(uint32_t sensor_id);

/*! Reads the current value for the given sensor
 * @param sensor_id The id of the sensor we want to calibrate
 * @return SMGR_RESULT::SMGR_SUCCESS Reading was successfull and data is stored
 * into reading
 * @return SMGR_RESULT::SMGR_UNKNOWN_SENSOR No sensor with sensor_id was found
 * (Note: reading will not get initialized)
 * @return SMGR_RESULT::SMGR_IO_ERROR We could not read from the sensor
 * hardware-wise (Note: reading will not get initialized)
 */
SMGR_RESULT sensor_manager_read_sensor(uint32_t sensor_id,
                                       sensor_reading *reading);

/*! Starts an RTOS-Task polling all registered sensors and updating the values.
 *
 * The time between the readings is configured through menuconfig under
"Components->Humidity Sensors->SENSOR_POLLING_TIME"
*/
SMGR_RESULT start_sensor_polling_task(void);

SMGR_RESULT
sensor_mgr_subscribe_new_sensor(new_sensor_callback callback);

/*! Register callback for calling if new measurements where taken
 * @param callback  */
SMGR_RESULT
sensor_mgr_subscribe_new_measurement(new_measurement_callback callback);
#endif
