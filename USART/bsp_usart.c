#include "bsp_usart.h"



void USART1_CONFIG(void){
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//配置映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	//配置 TX
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=USART1_TX;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&gpio);
	//配置 RX
	gpio.GPIO_Pin=USART1_RX;
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOA,&gpio);
	
	//配置usart
	usart.USART_BaudRate=115200;
	usart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	usart.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	usart.USART_Parity=USART_Parity_No;
	usart.USART_StopBits=USART_StopBits_1;
	usart.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&usart);
	
	//使能串口接受中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	//串口中断优先级的配置
	NVIC_CONFIG();
	//使能串口
	USART_Cmd(USART1,ENABLE);
}



void USART6_CONFIG(void){
	GPIO_InitTypeDef GPIO;
	USART_InitTypeDef USART;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_USART6);
	
	GPIO.GPIO_Mode=GPIO_Mode_AF;
	GPIO.GPIO_OType=GPIO_OType_PP;
	GPIO.GPIO_Pin=USART6_TX;
	GPIO.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO);
	
	GPIO.GPIO_OType=GPIO_OType_OD;
	GPIO.GPIO_Pin=USART6_RX;
	GPIO_Init(GPIOA,&GPIO);
	
	USART.USART_BaudRate=115200;
	USART.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART.USART_Parity=USART_Parity_No;
	USART.USART_StopBits=USART_StopBits_1;
	USART.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART6,&USART);
	//当 接受数据的寄存器不等于0 时
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
	
	NVIC_CONFIG();
	
	
	
	USART_Cmd(USART6,ENABLE);
	
	
	
}






void USART2_CONFIG(void){
	GPIO_InitTypeDef GPIO;
	USART_InitTypeDef usart;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	//配置映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_USART6);
	//配置 TX
	GPIO.GPIO_Mode=GPIO_Mode_AF;
	GPIO.GPIO_OType=GPIO_OType_PP;
	GPIO.GPIO_Pin=USART6_TX;
	GPIO.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO);
	
	//配置RX 
	GPIO.GPIO_Pin=USART6_RX;
	GPIO_Init(GPIOA,&GPIO);
	
	//配置usart
	usart.USART_BaudRate=115200;
	usart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	usart.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	usart.USART_Parity=USART_Parity_No;
	usart.USART_StopBits=USART_StopBits_1;
	usart.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART6,&usart);
	//对于USART6 进行使能
	USART_Cmd(USART6,ENABLE);
}






void NVIC_CONFIG(void){
	NVIC_InitTypeDef nvic;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	nvic.NVIC_IRQChannel=USART6_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority=1;
	nvic.NVIC_IRQChannelSubPriority=1;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic);
}


void Usart_SendByte(USART_TypeDef* USARTX,uint8_t data){
	USART_SendData(USARTX,data);
	while(USART_GetFlagStatus(USARTX,USART_FLAG_TXE)==RESET);
}



void Usart_SendStr(USART_TypeDef* USARTX,char* ch){
	u8 i=0;
	do{
		Usart_SendByte(USARTX,*(ch+i));
		i++;
	}while(*(ch+i)!='\0');
	while(USART_GetFlagStatus(USARTX,USART_FLAG_TC)==RESET);
}





int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口DEBUG_USART */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
		
		return (ch);
}




