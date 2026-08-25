idf_component_register(SRCS "PlantManager.c" "plant_repository.c" "sensor_reading_task.c" "sensor_data_task.c"
                    REQUIRES SensorManager RelaisDriver LEDManager UI BluetoothManager nvs_flash freertos
                    INCLUDE_DIRS "include")
