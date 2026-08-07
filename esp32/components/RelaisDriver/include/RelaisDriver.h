#include <stdint.h>

#define OPEN_LEVEL 0
#define CLOSE_LEVEL 1

void relais_drv_initialize(uint32_t relais_pin);
void relais_drv_open(void);
void relais_drv_close(void);
