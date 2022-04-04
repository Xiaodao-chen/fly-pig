#ifndef _BSP_TIM_H
#define _BSP_TIM_H
#include "stm32f4xx.h"

//TIM1 ��ʹ��Ϊ������ģ���ʹ�� PA9 ������TIM1_CH2����PPM
//TIM3 ��CH1,CH2,CH3,CH4 ���ֱ�����PWM�����  �����Ƶ�����Լ�������



#define Period 1000-1
#define Prescaler 83

#define TIM1_CH1_Pin  GPIO_Pin_8
#define TIM1_CH2_Pin  GPIO_Pin_9
#define TIM1_CH3_Pin  GPIO_Pin_10
#define TIM1_CH4_Pin  GPIO_Pin_11
#define TIM1_GPIO     GPIOA
#define TIM3_CH1_Pin  GPIO_Pin_6
#define TIM3_CH2_Pin  GPIO_Pin_7
#define TIM3_CH3_Pin  GPIO_Pin_0
#define TIM3_CH4_Pin  GPIO_Pin_1


#define TIM1_PULSE    6299

void TIM_Delay(int ti);
void TIM1_CONFIG(void);
void TIM3_CONFIG(void);


#endif 


/*
	������ ���������� ��
	������ ���������� С
	������ ���ĸ����� ��
	������ ���ĸ����� С
	ǰ��   �ڶ������� ��
	����   �ڶ������� С
	�ҽ�   ��һ������ ��
	���   ��һ������ С
*/

