#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H
/*! @file
 * Methods for handling connections over bluettoth
 */

/* Return codes for bluetooth-related methods */
typedef enum BLU_RESULT {
  BLU_SUCCESS = 0, /*!< Everything went fine */
  BLU_INITIALIZATION_ERROR =
      1, /*!< Some of the subsystems failed to initialize */
  BLU_TASK_CREATION_FAILED = 2, /*!< Task initialization failed */
} BLU_RESULT;

/*! Initializes the bluetooth-subsystem */
BLU_RESULT blu_initialize(void);

/*! Creates a new RTOS task */
BLU_RESULT create_task(void);

#endif // BLUETOOTH_MANAGER_H
