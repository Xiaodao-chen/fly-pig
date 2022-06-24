#ifndef _BSP_USART_H
#define _BSP_USART_H
#include "stm32f4xx.h"
#include <stdio.h>
//������Ĵ���1 ֻ��������ʵ�飬����������������� ���������������໥��ͻ�����Ե���ֻ��ʹ��USART6 �ش�˵��
//����F401 ���ڵ��ж�ֻ��usart1|2|6


#define USART1_GPIO GPIOA
#define USART1_TX  GPIO_Pin_9
#define USART1_RX  GPIO_Pin_10

//����6 ������ ���õ�PC6 USART_TX 
//PC7 USART_RX
#define USART6_TX  GPIO_Pin_6
#define USART6_RX  GPIO_Pin_7



void Usart_SendByte(USART_TypeDef* USARTX,uint8_t data);
void USART1_CONFIG(void);
void NVIC_CONFIG(void);
void Usart_SendStr(USART_TypeDef* USARTX,char* ch);
void NVIC_usart6_open(void);
void NVIC_usart6_close(void);
void Debug_USART_Config(void);
void USART6_CONFIG(void);
void I2C_Addr_Detect(void);
#endif




