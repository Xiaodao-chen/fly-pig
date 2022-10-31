#ifndef __BSP_PPM_H
#define __BSP_PPM_H
#include "stm32f4xx.h"
#include "bsp_oled.h"
#include <stdio.h>
#include "bsp_tim.h"

extern unsigned int T1,T2,T3,T4;
extern TIM_OCInitTypeDef CH1,CH2,CH3,CH4; //这是声明，在编译的时候找不到没有关系，但是在链接的时候去其他的源文件寻找
extern short TIM1_DataBuf[9];
/*
	需要修改输出的PWM的值 函数
*/

/*
header 
		|
		|
CH1   CH3
	\___/
	/		\
CH2   CH4

*/
void CH1_Change(int );
void CH2_Change(int );
void CH3_Change(int );
void CH4_Change(int );
void Show_PPM(void);
int  ChangeToPWM(int num);
void Fly_Start(void);
void PPM_Init(void);
void ChangeMotor(void);
#endif



