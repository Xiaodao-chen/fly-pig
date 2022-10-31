#include "bsp_usart.h"
#include "PID.h"
#include <string.h>
#include "PID.h"
char PID[50]={0};
char flag=0;


void USART1_CONFIG(void){
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=USART1_TX;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&gpio);
	gpio.GPIO_Pin=USART1_RX;
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOA,&gpio);
	usart.USART_BaudRate=115200;
	usart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	usart.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	usart.USART_Parity=USART_Parity_No;
	usart.USART_StopBits=USART_StopBits_1;
	usart.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&usart);
	//开启接受中断和空闲中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	//串口中断优先级的配置
	NVIC_InitTypeDef nvic;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvic.NVIC_IRQChannel=USART1_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority=1;
	nvic.NVIC_IRQChannelSubPriority=2;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic);
	//使能串口
	USART_Cmd(USART1,ENABLE);
}




void USART6_CONFIG(void){
	GPIO_InitTypeDef GPIO;
	USART_InitTypeDef USART;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);
	
	GPIO.GPIO_Mode=GPIO_Mode_AF;
	GPIO.GPIO_OType=GPIO_OType_PP;
	GPIO.GPIO_Pin=USART6_TX;
	GPIO.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO);
	
	GPIO.GPIO_OType=GPIO_OType_OD;
	GPIO.GPIO_Pin=USART6_RX;
	GPIO_Init(GPIOC,&GPIO);
	
	USART.USART_BaudRate=115200;
	USART.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART.USART_Parity=USART_Parity_No;
	USART.USART_StopBits=USART_StopBits_1;
	USART.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART6,&USART);
	//开启接受中断和空闲中断 和 传输完毕中断
	
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);
	
	USART_ITConfig(USART1,USART_IT_TC,DISABLE);
	NVIC_InitTypeDef nvic;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvic.NVIC_IRQChannel=USART6_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority=2;
	nvic.NVIC_IRQChannelSubPriority=1;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic);
	USART_Cmd(USART6,ENABLE);
}


void NVIC_usart6_open(void){
	NVIC_InitTypeDef nvic;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvic.NVIC_IRQChannel=USART6_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority=2;
	nvic.NVIC_IRQChannelSubPriority=2;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic);
}

void NVIC_usart6_close(void){
	NVIC_InitTypeDef nvic;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	nvic.NVIC_IRQChannel=USART6_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority=2;
	nvic.NVIC_IRQChannelSubPriority=2;
	nvic.NVIC_IRQChannelCmd=DISABLE;
	NVIC_Init(&nvic);
}



void Usart_SendByte(USART_TypeDef* USARTX,uint8_t data){
	USART_SendData(USARTX,data);
	while(USART_GetFlagStatus(USARTX,USART_FLAG_TXE)==RESET);
}



void Usart_SendStr(USART_TypeDef* USARTX,char* ch,uint8_t length){
	u8 i=0;
	while(length!=0){
		Usart_SendByte(USARTX,*(ch+i));
		i++;
		length--;
	}
	while(USART_GetFlagStatus(USARTX,USART_FLAG_TC)==RESET);
}


void Usart_SendStr1(USART_TypeDef* USARTX,char* ch){
	u8 i=0;
	while(ch[i]!='\0'){
		Usart_SendByte(USARTX,*(ch+i));
		i++;
	}
	while(USART_GetFlagStatus(USARTX,USART_FLAG_TC)==RESET);
}


int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口DEBUG_USART */
		USART_SendData(USART6, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);		
		
		return (ch);
}


//static void change(float* a,char b,char c){
//	*a=b*1.0f;
//	while(c!=0){
//		(*a)=(*a)/10;
//		c--;
//	}
//}

// 将USART_IRQHandler 数据接收

	void USART6_IRQHandler(){
		static int i=0;

		if(USART_GetITStatus(USART6,USART_IT_RXNE)!=RESET){
			PID[i]=USART_ReceiveData(USART6);
			i++;
		}else if(USART_GetITStatus(USART6,USART_IT_IDLE)!=RESET){
			//表示接受完毕
			USART6->SR;
			USART6->DR;
			if(i<10){
				flag=1;
			}
			i=0;
			
		}
}



