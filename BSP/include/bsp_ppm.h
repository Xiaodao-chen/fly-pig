#ifndef __BSP_PPM_H
#define __BSP_PPM_H
#include "stm32f4xx.h"
#include "bsp_oled.h"
#include <stdio.h>
#include "bsp_tim.h"

extern unsigned int T1,T2,T3,T4;
extern TIM_OCInitTypeDef CH1,CH2,CH3,CH4; //�����������ڱ����ʱ���Ҳ���û�й�ϵ�����������ӵ�ʱ��ȥ������Դ�ļ�Ѱ��
extern short TIM1_DataBuf[9];
/*
	��Ҫ�޸������PWM��ֵ ����
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



