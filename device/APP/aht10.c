#include "aht10.h"

/*******************************************/
/*@��Ȩ���У����ݰ��ɵ������޹�˾          */
/*@���ߣ���ʪ�ȴ�������ҵ��                */
/*@�汾��V1.2                              */
/*******************************************/
/*******************************************/
/*@�汾˵����                              */
/*@�汾�ţ�V1.2 �޸�AC����Ĳ�����         */
/*@�汾�ţ�V1.1 ����У׼���ʹ�ܼ�⡣     */
/*@�汾�ţ�V1.0 ����汾��                 */
/*******************************************/

const uint8_t DeviceAddress=0x38;


uint8_t JH_Read_Status(void)//��ȡAHT10��״̬�Ĵ���
{
	uint8_t Byte_first;	
	
	I2C_ReadByteWithoutRA(&Byte_first,1,  DeviceAddress);
	
	return Byte_first;
}

uint8_t JH_Read_Cal_Enable(void)  //��ѯcal enableλ��û��ʹ�ܣ�
{
	uint8_t val = 0;//ret = 0,
 
  val = JH_Read_Status();
  if((val & 0x68)==0x08)  //�ж�NORģʽ��У׼����Ƿ���Ч
  return 1;
  else  return 0;
 }



void JH_SendAC(void) //��AHT10����AC����
{
	uint8_t buffer[3]={0xac,0x33,0x00};

	I2C_WriteByteWithoutWA(buffer,3,DeviceAddress);
}

void JH_Send_BA(void)//��AHT10����BA����
{
	uint8_t buffer=0xba;

	I2C_WriteByteWithoutWA(&buffer,1,DeviceAddress);
}
/*
#define Byte_1th Bbyte[0]
#define Byte_2th Bbyte[1]
#define Byte_3th Bbyte[2]
#define Byte_4th Bbyte[3]
#define Byte_5th Bbyte[4]
#define Byte_6th Bbyte[5]*/
void JH_Read_CTdata(uint32_t *ct) //��ȡAHT10���¶Ⱥ�ʪ������
{
	volatile uint8_t Bbyte[6]={0};
	 uint32_t RetuData = 0;
	
	uint16_t cnt = 0;


	JH_SendAC();//��AHT10����AC����

	delay_ms(75);//�ȴ�75ms
    cnt = 0;
	while(((JH_Read_Status()&0x80)==0x80))//�ȴ�æ״̬����
	{
	 delay_us(1508);
	 if(cnt++>=100)
	 {
	  break;
	  }
	}

	I2C_ReadByteWithoutRA((uint8_t*)Bbyte,6,DeviceAddress);
	
	RetuData = (RetuData|Bbyte[1])<<8;
	RetuData = (RetuData|Bbyte[2])<<8;
	RetuData = (RetuData|Bbyte[3]);
	RetuData =RetuData >>4;
	ct[0] = RetuData;
	RetuData = 0;
	RetuData = (RetuData|Bbyte[3])<<8;
	RetuData = (RetuData|Bbyte[4])<<8;
	RetuData = (RetuData|Bbyte[5]);
	RetuData = RetuData&0xfffff;
	ct[1] =RetuData; 

}
/*
#undef Byte_1th
#undef Byte_2th
#undef Byte_3th
#undef Byte_4th
#undef Byte_5th
#undef Byte_6th*/

u8 JH_Init(void)   //��ʼ��AHT10
{
	uint8_t buffer[3]={0xe1,0x08,0x00};
	uint8_t count=0;
	
	delay_us(11038);

	I2C_WriteByteWithoutWA(buffer,3,DeviceAddress);
	
	delay_ms(500);//��ʱ0.5S
   while(JH_Read_Cal_Enable()==0)//��Ҫ�ȴ�״̬��status��Bit[3]=1ʱ��ȥ�����ݡ����Bit[3]������1 ���������λ0xBA��AHT10�������³�ʼ��AHT10��ֱ��Bit[3]=1
   {
		JH_Send_BA();  //��λ
		delay_ms(100);
		delay_us(11038);

		I2C_WriteByteWithoutWA(buffer,3,DeviceAddress);
		 
		count++;
		if(count>=10)return 0;
		delay_ms(500);
   }
	 return 1;
}
int8_t aht10init(void){
	u8 ret=0;
	I2C_Configuration();
	ret = JH_Init(); //��ʼ��
	if(ret == 0) return -1;
	return 0;
}
int8_t getaht10data(int *humidity,int *temperature)
{
 uint32_t CT_data[2];
	volatile int  c1,t1;
	uint8_t count=0;
	
	 while(JH_Read_Cal_Enable()==0)//�ȵ�У׼���ʹ��λΪ1���Ŷ�ȡ��
	 {
	  JH_Init();//���Ϊ0��ʹ��һ��
      delay_ms(30);
		 if(count++>10) return -1;
	 }
	 //DisableIrq(); //������ģ��I2C,Ϊ��ȡ���ݸ�׼ȷ ����ȡ֮ǰ��ֹ�ж�
	 JH_Read_CTdata(CT_data);  //��ȡ�¶Ⱥ�ʪ�� �� �ɼ��1.5S��һ��
	 c1 = CT_data[0]*1000/1024/1024;  //����õ�ʪ��ֵ���Ŵ���10��,���c1=523����ʾ����ʪ��Ϊ52.3%��
	 t1 = CT_data[1] *200*10/1024/1024-500;//����õ��¶�ֵ���Ŵ���10�������t1=245����ʾ�����¶�Ϊ24.5�棩
	 //EnableIrq(); //�ָ��ж�
	 *humidity=c1;
	 *temperature=t1;
	 return 0;
	 //delay_ms(1500); //��ʱ1.5S
	 
	 //Ϊ��ȡ�����ݸ��ȶ���������ʹ��ƽ��ֵ�˲����ߴ����˲�������ǰ���ȡ��ֵ������ֵ����̫��
	 
 }	
