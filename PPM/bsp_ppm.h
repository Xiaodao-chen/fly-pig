#ifndef __BSP_PPM_H
#define __BSP_PPM_H
#include "stm32f4xx.h"
#include "bsp_ppm.h"
#include "bsp_oled.h"
#include <stdio.h>
#include "bsp_tim.h"

/*
	需要修改输出的PWM的值 函数
*/

void CH1_Change(void);
void CH2_Change(void);
void CH3_Change(void);
void CH4_Change(void);
void Show_PPM(void);

#endif



