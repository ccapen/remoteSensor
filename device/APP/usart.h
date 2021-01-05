#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h"
void usart1_init(void);
uint8_t SendBytesUSART1(const uint8_t *pBuffer,uint8_t length);
#endif
