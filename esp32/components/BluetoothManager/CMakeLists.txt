idf_component_register(
  SRCS "BluetoothManager.c" "gap.c" "gatt_svc.c" "heart_rate_mock.c" "sensor_data_handler.c"
  PRIV_REQUIRES bt nvs_flash RGBLEDDriver SensorManager
  INCLUDE_DIRS "include")
