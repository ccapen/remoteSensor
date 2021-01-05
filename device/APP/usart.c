#include "usart.h"
void usart1_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=19200;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1,ENABLE);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	//NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	//NVIC_Init(&NVIC_InitStructure);
}
uint8_t SendBytesUSART1(const uint8_t *pBuffer,uint8_t length){
		while(length--){
			unsigned char tmp=*pBuffer;
			USART_ClearFlag(USART1,USART_FLAG_TC);
			USART_SendData(USART1,tmp);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
			pBuffer++;
		}
		return 0;
}
