#include "hal/adc_types.h"
#include <esp_adc/adc_oneshot.h>
#include <sdkconfig.h>

adc_atten_t get_sdkconfig_attentuation() {
#ifdef SENSOR_ATTEN_0
  return ADC_ATTEN_DB_0;
#endif

#ifdef SENSOR_ATTEN_2_5
  return ADC_ATTEN_DB_2_5;
#endif

#ifdef SENSOR_ATTEN_6
  return ADC_ATTEN_DB_6;
#endif

#ifdef SENSOR_ATTEN_11
  return ADC_ATTEN_DB_11;
#endif
  // Fallback
  return ADC_ATTEN_DB_0;
}
