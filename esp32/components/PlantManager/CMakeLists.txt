idf_component_register(SRCS "PlantManager.c" "plant_repository.c"
                    PRIV_REQUIRES SensorManager RelaisDriver UI BluetoothManager nvs_flash
                    INCLUDE_DIRS "include")
