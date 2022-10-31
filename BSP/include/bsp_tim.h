#ifndef _BSP_TIM_H
#define _BSP_TIM_H
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
/*
TIM1 被使用为接收器模块的使用 PA9 被用作TIM1_CH2接受PPM
TIM3 的CH1,CH2,CH3,CH4 被分别用于PWM的输出  输出的频率是自己决定的
TIM3 的CH1,CH2,CH3,CH4 分别是 PA6,PA7,PB0,PB1
*/


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
void TIM2_CONFIG(void);
void TIM3_CONFIG(void);
void TIM4_CONFIG(void);
float GY86_GETTIME(void);
float PID_GETTIME(void);
void TIM5_CONFIG(void);

void TIMDelay(uint32_t Times);
#endif 


/*
	油门上 第三个波峰 大
	油门下 第三个波峰 小
	方向右 第四个波峰 大
	方向右 第四个波峰 小
	前进   第二个波峰 大
	后退   第二个波峰 小
	右进   第一个波峰 大
	左进   第一个波峰 小
*/

