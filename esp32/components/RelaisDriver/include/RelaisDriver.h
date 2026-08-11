#include "relais_types.h"
#include <esp_err.h>
#include <stdint.h>

#define OPEN_LEVEL 0
#define CLOSE_LEVEL 1

/*! Initializes the internal storage of all registered relais.
 * @param initial_number The internal storage will initially allocate enough
 *        memory for the given number of sensors
 * @remark The initial number of sensors is just used to avoid the need of
 *         dynamic allocation. You can add more than initial_number relais later
 * (or even initialize 0 elements), but this will come with the caveat that
 * registering new relais will take more time, since memory had to get
 * reallocated.
 */
esp_err_t relais_drv_initialize(uint32_t initial_number);

/*! Initializes the driver for controlling the relais at the given GPIO
 * @param[in] relais_gpio GPIO the relais is connected to
 * @param[out] handle relais_handle to use to control the relais
 */
esp_err_t relais_drv_add_relais(uint32_t relais_gpio, relais_handle_T *handle);

/*! Opens the relais managed by handle
 * @param handle The handle of the relais to open
 */
esp_err_t relais_drv_open(relais_handle_T handle);

/*! Closes the relais managed by handle
 * @param handle The handle of the relais to open
 */
esp_err_t relais_drv_close(relais_handle_T handle);
