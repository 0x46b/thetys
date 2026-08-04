#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H
/*! @file
Methods for managing the humidity-sensors
*/

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

/*! Initializes the sensor-manager.
 * Sets the correct data-directions, allocates memory etc.
 * @return SMGR_RESULT::SMGR_SUCCESS Configurations successfully initialized
 * @return SMGR_RESULT::SMGR_CATASTROPHIC_FAILURE Something went terribly wrong
 */
SMGR_RESULT initialize(uint32_t number_of_sensors);

/*! Adds a sensor to the sensor-manager for monitoring
 * @param sensor_pin The GPIO where the sensor is attached to
 * @return SMGR_RESULT::SMGR_SUCCESS Sensor added successfully
 * @return SMGR_RESULT::SMGR_CATASTROPHIC_FAILURE Something went terribly wrong
 */
SMGR_RESULT add_sensor(uint32_t sensor_pin);

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
SMGR_RESULT read_sensor_data(uint32_t sensor_id, sensor_reading *reading);
#endif
