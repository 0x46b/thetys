/*! @file
 * API for managing sensors.
 */

/* Copyright (C) 2026 Sebastian Murschall <sebastian.murschall@gmail.com>
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

#include "sensor_mgr_type_definitions.h"
#include "sensor_register.h"
#include <stdint.h>

/*! Callback definition for a new data callback. Subscribe via
 * sensor_mgr_subscribe_new_measurement */
typedef void (*new_measurement_callback_t)(sensor_reading_T);

/*! Callback definition for a new sensor callback. Subscribe via
 * sensor_mgr_subscribe_new_sensor */
typedef void (*new_sensor_callback_t)(sensor_T);

/*! Initializes the sensor-manager.
 * Sets the correct data-directions, allocates memory etc.
 * @return SMGR_RESULT::SMGR_SUCCESS Configurations successfully initialized
 * @return SMGR_RESULT::SMGR_CATASTROPHIC_FAILURE Something went terribly wrong
 */
SMGR_RESULT sensor_mgr_initialize(uint32_t number_of_sensors);

/*! Adds a sensor to the sensor-manager for monitoring
 * @param sensor_pin The GPIO where the sensor is attached to
 * @param[out] sensor_handle Handle to the registered sensor
 * @return SMGR_RESULT::SMGR_SUCCESS Sensor added successfully
 * @return SMGR_RESULT::SMGR_CATASTROPHIC_FAILURE Something went terribly wrong
 */
SMGR_RESULT sensor_mgr_add_sensor(uint32_t sensor_pin,
                                  sensor_handle_T *sensor_handle);

/*! Reads the current value for the given sensor
 * @param sensor_handle The handle for the sensor we want to calibrate
 * @return SMGR_RESULT::SMGR_SUCCESS Reading was successfull and data is stored
 * into reading
 * @return SMGR_RESULT::SMGR_UNKNOWN_SENSOR No sensor with sensor_id was found
 * (Note: reading will not get initialized)
 * @return SMGR_RESULT::SMGR_IO_ERROR We could not read from the sensor
 * hardware-wise (Note: reading will not get initialized)
 */
SMGR_RESULT sensor_manager_read_sensor(sensor_handle_T sensor_handle,
                                       sensor_reading_T *reading);

/*! Starts an RTOS-Task polling all registered sensors and updating the values.
 *
 * The time between the readings is configured through menuconfig under
"Components->Humidity Sensors->SENSOR_POLLING_TIME"
*/
SMGR_RESULT sensor_mgr_start_polling_task(void);

SMGR_RESULT
sensor_mgr_subscribe_new_sensor(new_sensor_callback_t callback);

/*! Register callback that gets called if new measurements where taken
 * @param callback  */
SMGR_RESULT
sensor_mgr_subscribe_new_measurement(new_measurement_callback_t callback);
#endif
