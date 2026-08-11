/*! @file
 * PlantManager orchestrates the different other low-level-APIs to build a
 * higher-level front to the user.
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
#ifndef PLANTMANAGER_H
#define PLANTMANAGER_H

#include "esp_err.h"
#include "plant_mgr_type_definitions.h"

/*! Initializes the subsystems (like the sensor-drivers etc.) and allocates
 *  needed data-structures. Had to get called before calling any other method.
 * @return ESP_OK Everything went fine, all systems are ready to use
 * @return ESP_FAIL Something went wrong, maybe a driver could not initialize or
 * something. Check logs for more information
 */
esp_err_t plant_mgr_initialize(void);

/*! Registers a new plant in the internal storage and initializes everything for
 * the underlying drivers to handle every interaction (reading sensors,
 * controlling pumps, updating the UI etc.)
 * @param[in] plant_name The name of the plant
 * @param[in] pump_gpio GPIO the underlying RelaisDriver can control the pump
 * pump with
 * @param[in] sensor_gpio GPIO the underlying SensorMgr to be able to interact
 * with the sensor
 * @param[in] watering_threshold If a measurement is lower then the given
 * threshold watering will get initialized
 * @param[out] created_plant The created plant_T structure with all
 * informations. Could be NULL if not needed.
 */
esp_err_t plant_mgr_add_plant(const char *plant_name, uint32_t pump_gpio,
                              uint32_t sensor_gpio, uint32_t watering_threshold,
                              plant_T *created_plant);

/*! Removes the plant from the internal storage and all handling of it.
 * @param remove_plant The plant_T that should no longer managed through the
 * PlantManager
 */
esp_err_t plant_mgr_remove_plant(plant_T *remove_plant);
esp_err_t plant_mgr_set_plant_status(plant_handle_T plant, bool is_active);

/*! Start the management. Will spawn threads for periodically reading the
 * sensors, updating the UI and starting the relevant pumps, if needed */
esp_err_t plant_mgr_start(void);

#endif // PLANTMANAGER_H
