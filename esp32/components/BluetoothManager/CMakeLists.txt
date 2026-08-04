idf_component_register(
  SRCS "BluetoothManager.c" "gap.c" "gatt_svc.c" "heart_rate_mock.c"
  PRIV_REQUIRES bt nvs_flash RGBLEDDriver
  INCLUDE_DIRS "include")
