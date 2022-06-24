#include "bsp_ray.h"
#include "bsp_systick.h"
#include "bsp_oled.h"
#include "bsp_led.h"
#include "bsp_usart.h"
uint16_t HEIGHT[9]={0};
char a1[50]={'0'};
char b1[50]={'0'};
char c1[50]={'0'};
char d1[50]={0};
char e1[50]={0};
char f1[50]={0};
char g1[50]={0};
#if I2C_MODE
void Get_Hight(){
	while(I2C_GetFlagStatus(TF_I2C,I2C_FLAG_BUSY)==1);
	I2C_GenerateSTART(TF_I2C,ENABLE);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	I2C_Send7bitAddress(TF_I2C,TF_ADD_Write,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	I2C_SendData(TF_I2C,0X5A);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	I2C_SendData(TF_I2C,0X05);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	I2C_SendData(TF_I2C,0X00);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	I2C_SendData(TF_I2C,0X06);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	I2C_SendData(TF_I2C,0X65);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	I2C_GenerateSTOP(TF_I2C,ENABLE);
	Systick_Delay_ms(100);
	I2C_GenerateSTART(TF_I2C,ENABLE);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	I2C_Send7bitAddress(TF_I2C,TF_ADD_Read,I2C_Direction_Receiver);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==ERROR){};
	for(int i=0;i<9;i++){
		Systick_Delay_ms(5);
		HEIGHT[i]=I2C_ReceiveData(TF_I2C);
		I2C3->CR1|=(1<<10);
	}
	I2C3->CR1&=~(1<<10);
	I2C_GenerateSTOP(TF_I2C,ENABLE);
}

void TF_Commands(uint8_t* commands,int length){
	while(I2C_GetFlagStatus(TF_I2C,I2C_FLAG_BUSY)==1);
	I2C_GenerateSTART(TF_I2C,ENABLE);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	I2C_Send7bitAddress(TF_I2C,TF_ADD_Write,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	for(int i=0;i<length;i++){
		I2C_SendData(TF_I2C,commands[i]);	
		while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);	
	}
	I2C_GenerateSTOP(TF_I2C,ENABLE);
	Systick_Delay_ms(100);
	I2C_GenerateSTART(TF_I2C,ENABLE);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	I2C_Send7bitAddress(TF_I2C,TF_ADD_Read,I2C_Direction_Receiver);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==ERROR){};
	for(int i=0;i<9;i++){
		Systick_Delay_ms(5);
		HEIGHT[i]=I2C_ReceiveData(TF_I2C);
		I2C3->CR1|=(1<<10);
	}
	I2C3->CR1&=~(1<<10);
	I2C_GenerateSTOP(TF_I2C,ENABLE);
}


void TF_Init(void){
	while(I2C_GetFlagStatus(TF_I2C,I2C_FLAG_BUSY)==1);
	I2C_GenerateSTART(TF_I2C,ENABLE);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	I2C_Send7bitAddress(TF_I2C,0x00,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	I2C_SendData(TF_I2C,0X06);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	I2C_GenerateSTOP(TF_I2C,ENABLE);
}


void Set_TF(uint8_t register_h,uint8_t register_l ,uint8_t command){
	while(I2C_GetFlagStatus(TF_I2C,I2C_FLAG_BUSY)==1);
	I2C_GenerateSTART(TF_I2C,ENABLE);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	I2C_Send7bitAddress(TF_I2C,TF_ADD_Write,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	I2C_SendData(TF_I2C,register_h);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	I2C_SendData(TF_I2C,register_l);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	//指令长度
	I2C_SendData(TF_I2C,0X01);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR){};
	I2C_GenerateSTART(TF_I2C,ENABLE);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	I2C_Send7bitAddress(TF_I2C,TF_ADD_Write,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	I2C_SendData(TF_I2C,0X00);
	while(I2C_CheckEvent(TF_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR){};
	I2C_GenerateSTOP(TF_I2C,ENABLE);

}

#endif
#if USART_MODE
	void USART1_IRQHandler(){
		static int i=0;
		if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
			HEIGHT[i]=USART_ReceiveData(USART1);
			i++;
		}else if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET){
			//表示接受完毕
			USART1->SR;
			USART1->DR;
			i=0;
		}
	}
#endif















void HEIGHT_SHOW(){
	int hight=HEIGHT[3]<<8|HEIGHT[2];
	int strength=(HEIGHT[5]<<8)+HEIGHT[4];
	int temp=((HEIGHT[7]<<8)+HEIGHT[6])/8-256;
	sprintf(d1,"Dist is %d",hight);
	sprintf(e1,"Strength is %d",strength);
	sprintf(f1,"Temp is %d",temp);
	OLED_ShowStr(0,4,d1);
	OLED_ShowStr(0,5,e1);
	OLED_ShowStr(0,6,f1);
		
}






