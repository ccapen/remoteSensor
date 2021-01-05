#ifndef AHT10_H_
#define AHT10_H_
#include "iic.h"
#include "systick.h"
int8_t aht10init(void);
int8_t getaht10data(int *humidity,int *temperature);
#endif
