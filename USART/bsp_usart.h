#ifndef _BSP_USART_H
#define _BSP_USART_H
#include "stm32f4xx.h"
#include <stdio.h>
//对于这的串口1 只是用于做实验，在四轴飞行器中由于 他与其他的引脚相互冲突，所以导致只能使用USART6 特此说明
//对于F401 串口的中断只有usart1|2|6


#define USART1_GPIO GPIOA
#define USART1_TX  GPIO_Pin_9
#define USART1_RX  GPIO_Pin_10

//串口6 的配置
#define USART6_TX  GPIO_Pin_11
#define USART6_RX  GPIO_Pin_12







void Usart_SendByte(USART_TypeDef* USARTX,uint8_t data);
void USART1_CONFIG(void);
void USART2_CONFIG(void);
void NVIC_CONFIG(void);
void Usart_SendStr(USART_TypeDef* USARTX,char* ch);
void NVIC_CONFIG(void);
void Debug_USART_Config(void);

void USART6_CONFIG(void);
#endif


