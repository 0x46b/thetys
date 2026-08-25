idf_component_register(
  SRCS "BluetoothManager.c" "gap.c" "gatt_svc.c" "plant_service.c"
  PRIV_REQUIRES bt nvs_flash RGBLEDDriver PlantManager cjson
  INCLUDE_DIRS "include")
