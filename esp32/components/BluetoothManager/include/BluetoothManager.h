#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H
#include <stdint.h>

/*! @file
 * Methods for handling connections over bluettoth
 */
/* Return codes for bluetooth-related methods */

#define BLUETOOTH_DEVICE_NAME "Thetys Remote Control"
#define PLANT_DATA_UPDATE_PERIOD_MS 1000
#define BLUETOOTH_CPU_CORE 1

typedef enum BLU_RESULT {
  BLU_OK = 0, /*!< Everything went fine */
  BLU_INITIALIZATION_ERROR =
      1, /*!< Some of the subsystems failed to initialize */
  BLU_TASK_CREATION_FAILED = 2, /*!< Task initialization failed */
  BLU_CALLBACK_ALREADY_SUBSCRIBED = 3,
} BLU_RESULT;

typedef void (*new_plant_callback_t)(const char *plant_name,
                                     uint32_t sensor_gpio, uint32_t pump_gpio,
                                     uint32_t watering_threshold);

/*! Initializes the bluetooth-subsystem */
BLU_RESULT blu_initialize(void);
BLU_RESULT blue_subscribe_new_plant(new_plant_callback_t callback);

/*! Creates a new RTOS task */
BLU_RESULT create_task(void);

#endif // BLUETOOTH_MANAGER_H
