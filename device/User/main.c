#include "public.h"

void swapendian(uint8_t *pBuffer,uint8_t length){
	uint8_t *ptmp=pBuffer+length-1,tmp;
	length/=2;
	while(length--){
		tmp=*pBuffer;
		*pBuffer=*ptmp;
		*ptmp=tmp;
		pBuffer++;
		ptmp--;
	}
}
int main(){
	int temp,humi;
	usart1_init();
	bmp280init();
	aht10init();
	while(1){
		uint32_t pres=getpres(puint64);
		getaht10data(&humi,&temp);
		//uint32_t k=gettempint32();
		//uint32_t k=(uint32_t)getpresdoub();
		//swapendian((uint8_t*)&pres,sizeof(uint32_t));
		SendBytesUSART1((uint8_t*)&pres,sizeof(uint32_t));
		//swapendian((uint8_t*)&humi,sizeof(int));
		SendBytesUSART1((uint8_t*)&humi,sizeof(int));
		//swapendian((uint8_t*)&temp,sizeof(int));
		SendBytesUSART1((uint8_t*)&temp,sizeof(int));
		
		delay_ms(1000);
	}
}
