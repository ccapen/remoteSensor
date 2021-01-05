#include "aht10.h"

/*******************************************/
/*@版权所有：广州奥松电子有限公司          */
/*@作者：温湿度传感器事业部                */
/*@版本：V1.2                              */
/*******************************************/
/*******************************************/
/*@版本说明：                              */
/*@版本号：V1.2 修改AC命令的参数。         */
/*@版本号：V1.1 增加校准输出使能检测。     */
/*@版本号：V1.0 最初版本。                 */
/*******************************************/

const uint8_t DeviceAddress=0x38;


uint8_t JH_Read_Status(void)//读取AHT10的状态寄存器
{
	uint8_t Byte_first;	
	
	I2C_ReadByteWithoutRA(&Byte_first,1,  DeviceAddress);
	
	return Byte_first;
}

uint8_t JH_Read_Cal_Enable(void)  //查询cal enable位有没有使能？
{
	uint8_t val = 0;//ret = 0,
 
  val = JH_Read_Status();
  if((val & 0x68)==0x08)  //判断NOR模式和校准输出是否有效
  return 1;
  else  return 0;
 }



void JH_SendAC(void) //向AHT10发送AC命令
{
	uint8_t buffer[3]={0xac,0x33,0x00};

	I2C_WriteByteWithoutWA(buffer,3,DeviceAddress);
}

void JH_Send_BA(void)//向AHT10发送BA命令
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
void JH_Read_CTdata(uint32_t *ct) //读取AHT10的温度和湿度数据
{
	volatile uint8_t Bbyte[6]={0};
	 uint32_t RetuData = 0;
	
	uint16_t cnt = 0;


	JH_SendAC();//向AHT10发送AC命令

	delay_ms(75);//等待75ms
    cnt = 0;
	while(((JH_Read_Status()&0x80)==0x80))//等待忙状态结束
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

u8 JH_Init(void)   //初始化AHT10
{
	uint8_t buffer[3]={0xe1,0x08,0x00};
	uint8_t count=0;
	
	delay_us(11038);

	I2C_WriteByteWithoutWA(buffer,3,DeviceAddress);
	
	delay_ms(500);//延时0.5S
   while(JH_Read_Cal_Enable()==0)//需要等待状态字status的Bit[3]=1时才去读数据。如果Bit[3]不等于1 ，发软件复位0xBA给AHT10，再重新初始化AHT10，直至Bit[3]=1
   {
		JH_Send_BA();  //复位
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
	ret = JH_Init(); //初始化
	if(ret == 0) return -1;
	return 0;
}
int8_t getaht10data(int *humidity,int *temperature)
{
 uint32_t CT_data[2];
	volatile int  c1,t1;
	uint8_t count=0;
	
	 while(JH_Read_Cal_Enable()==0)//等到校准输出使能位为1，才读取。
	 {
	  JH_Init();//如果为0再使能一次
      delay_ms(30);
		 if(count++>10) return -1;
	 }
	 //DisableIrq(); //由于是模拟I2C,为读取数据更准确 ，读取之前禁止中断
	 JH_Read_CTdata(CT_data);  //读取温度和湿度 ， 可间隔1.5S读一次
	 c1 = CT_data[0]*1000/1024/1024;  //计算得到湿度值（放大了10倍,如果c1=523，表示现在湿度为52.3%）
	 t1 = CT_data[1] *200*10/1024/1024-500;//计算得到温度值（放大了10倍，如果t1=245，表示现在温度为24.5℃）
	 //EnableIrq(); //恢复中断
	 *humidity=c1;
	 *temperature=t1;
	 return 0;
	 //delay_ms(1500); //延时1.5S
	 
	 //为读取的数据更稳定，还可以使用平均值滤波或者窗口滤波，或者前面读取的值与后面的值相差不能太大。
	 
 }	
