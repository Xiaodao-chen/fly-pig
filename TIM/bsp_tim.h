#ifndef _BSP_TIM_H
#define _BSP_TIM_H
#include "stm32f4xx.h"

//TIM1 被使用为接收器模块的使用
//TIM2 被使用为 电调的使用
//TIM4 是作为计时器使用
//PWM 信号的频率 F=TIM_CLK/(ARR+1)*(RSC+1)
//PWM 频率计算  时钟频率/(TIM_Period+1)
// 占空比的计算 (Period+1)* 占空比-1= 设置PULSE 



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


