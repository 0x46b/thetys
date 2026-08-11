/*! @file
 * Typedefinitions used in the SensorManager
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
#ifndef SENSOR_MGR_TYPE_DEFINITIONS_H
#define SENSOR_MGR_TYPE_DEFINITIONS_H

#include <stdint.h>
#include <time.h>

/*! Operation result codes */
typedef enum SMGR_RESULT {
  SMGR_SUCCESS = 0,  /*!< Everything went as expected */
  SMGR_IO_ERROR = 1, /*!< The sensor is not readably due hardware-failure */
  SMGR_UNKNOWN_SENSOR =
      2, /*!< The sensor with the given id is not registered for handling */
  SMGR_MISSING_CALIBRATION = 3, /*!< Calibration-data for sensor is missing */
  SMGR_CATASTROPHIC_FAILURE =
      4, /*!< Something went terribly wrong, so we could not recover from it */
  SMGR_CALLBACK_ALREADY_ASSIGNED =
      5, /*!< Try to subscribe to an already assigned callback */
  SMGR_UNKNOWN_ERROR = 255 /*!< Something went wrong, but we dunno what */
} SMGR_RESULT;

/*! Handle to a registered sensor */
typedef uint32_t sensor_handle_T;

/*! Struct for holding a sensor-reading */
typedef struct sensor_reading_T {
  time_t timestamp;          /*!< Timestamp of the reading */
  uint32_t sensor_id;        /*!< The id of the corresponding sensor */
  float humidity_percentage; /*!< The humidity in percentage (0..1) */
} sensor_reading_T;

#endif // SENSOR_MGR_TYPE_DEFINITIONS_H
