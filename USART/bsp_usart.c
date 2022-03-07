#include "bsp_usart.h"



void USART1_CONFIG(void){
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//����ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	//���� TX
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=USART1_TX;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&gpio);
	//���� RX
	gpio.GPIO_Pin=USART1_RX;
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOA,&gpio);
	
	//����usart
	usart.USART_BaudRate=115200;
	usart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	usart.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	usart.USART_Parity=USART_Parity_No;
	usart.USART_StopBits=USART_StopBits_1;
	usart.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&usart);
	
	//ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	//�����ж����ȼ�������
	NVIC_CONFIG();
	//ʹ�ܴ���
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
	//�� �������ݵļĴ���������0 ʱ
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
	
	NVIC_CONFIG();
	
	
	
	USART_Cmd(USART6,ENABLE);
	
	
	
}






void USART2_CONFIG(void){
	GPIO_InitTypeDef GPIO;
	USART_InitTypeDef usart;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	//����ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_USART6);
	//���� TX
	GPIO.GPIO_Mode=GPIO_Mode_AF;
	GPIO.GPIO_OType=GPIO_OType_PP;
	GPIO.GPIO_Pin=USART6_TX;
	GPIO.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO);
	
	//����RX 
	GPIO.GPIO_Pin=USART6_RX;
	GPIO_Init(GPIOA,&GPIO);
	
	//����usart
	usart.USART_BaudRate=115200;
	usart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	usart.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	usart.USART_Parity=USART_Parity_No;
	usart.USART_StopBits=USART_StopBits_1;
	usart.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART6,&usart);
	//����USART6 ����ʹ��
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
		/* ����һ���ֽ����ݵ�����DEBUG_USART */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
		
		return (ch);
}




