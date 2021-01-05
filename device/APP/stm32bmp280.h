#ifndef STM32BMP280_H_
#define STM32BMP280_H_
#include "bmp280.h"
#include "systick.h"
#include "iic.h"
enum prestype{
	pucompuint32,
	puint32,
	puint64,
	pdouble};

void bmp280init(void);
	
uint32_t getpres(enum prestype a);
double getpresdoub(void);
uint32_t gettempint32(void);
double gettempdou(void);
	
#endif
