#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_tim.h"
#include "bsp_oled.h"
#include "bsp_systick.h"
#include "bsp_esp8266.h"
#include "bsp_gy86.h"
#include <stdio.h>
#include "bsp_ppm.h"
#include "bsp_ray.h"
#include "ucos_ii.h"
#include "os_cpu.h"
#include "os_cfg.h"
#include "datafusion.h"
#include "ucos_ii.h"
#include "os_cpu.h"
#include "os_cfg.h"






extern mpu_Struct mpu;
extern hmc_Struct hmc;
extern char esp[20];
int m=0;
int time=0;

#define GY86_THREAD    7
#define GY86_SIZE      256
OS_STK GETDATA_thread_stack[GY86_SIZE];

#define Control_Motor  8
#define Control_SIZE   128
OS_STK Control_thread_stack[Control_SIZE];

#define OLED_THREAD    9
#define OLED_SIZE      256
OS_STK OLED_thread_stack[OLED_SIZE];

#define ANO_Thread   	 10
#define ANO_SIZE     	 128
OS_STK ANO_thread_stack[ANO_SIZE];



/*
在所有外设已经调整完成之后，开始写所使用到的引脚
首先是TIM1 接收 使用的是 TIM_CH4 也就是 GPIOA 11
雷达使用PA9 和PA10进行
*/

void GY86_Thread(void* a ){
	a=a;
	while(1){
	LED_OFF();
	GY86_getdata();
	MPU_Show();
	HMC_Show();
	LED_ON();
	OSTimeDly(2);
	}
}

void OLED_Thread(void* a){
	a=a;
	while(1){
		OSTimeDly(3);
	}
}

void MY_Init(void){
	I2C1_CONFIG();
	I2C_DeInit(I2C3);
	I2C3_CONFIG();
	OLED_Init();
	TIM1_CONFIG();
	TIM3_CONFIG();
	TIM4_CONFIG();
	LED_CONFIG();
	USART1_CONFIG();
	USART6_CONFIG();
	MPU_Init();
	HMC_Init();
	PPM_Init();
	OLED_PRINT("Ready",2);
	Fly_Start();
}

int main(){
	MY_Init();
	SysTick_Config(SystemCoreClock/1000);
	OSInit();  
	LED_ON();
	OSTaskCreate(GY86_Thread,(void *)0,(OS_STK *)&GETDATA_thread_stack[GY86_SIZE-1],GY86_THREAD);
	OSTaskCreate(OLED_Thread,(void *)0,(OS_STK *)&OLED_thread_stack[OLED_SIZE-1],OLED_THREAD );
	OSStart();
	
}




