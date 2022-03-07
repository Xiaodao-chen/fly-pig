#ifndef _BSP_TIM_H
#define _BSP_TIM_H
#include "stm32f4xx.h"

//TIM1 ��ʹ��Ϊ������ģ���ʹ��
//TIM2 ��ʹ��Ϊ �����ʹ��
//TIM4 ����Ϊ��ʱ��ʹ��
//PWM �źŵ�Ƶ�� F=TIM_CLK/(ARR+1)*(RSC+1)
//PWM Ƶ�ʼ���  ʱ��Ƶ��/(TIM_Period+1)
// ռ�ձȵļ��� (Period+1)* ռ�ձ�-1= ����PULSE 



#define Period 1000-1
#define Prescaler 83

#define TIM1_CH1_Pin  GPIO_Pin_8
#define TIM1_CH2_Pin  GPIO_Pin_9
#define TIM1_CH3_Pin  GPIO_Pin_10
#define TIM1_CH4_Pin  GPIO_Pin_11
#define TIM1_GPIO     GPIOA

#define TIM1_PULSE    6299




void TIM4_Init(void);
void TIM_Delay(int ti);
void TIM1_COFIG(void);

#endif 


