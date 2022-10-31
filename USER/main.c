#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_tim.h"
#include "bsp_oled.h"
#include "bsp_systick.h"
#include "bsp_esp8266.h"
#include "bsp_gy86.h"
#include "bsp_ppm.h"
#include <stdio.h>
#include "bsp_ppm.h"
#include "bsp_ray.h"
#include "ucos_ii.h"
#include "os_cpu.h"
#include "os_cfg.h"
#include "datafusion.h"
#include "ucos_ii.h"
#include "ANO.h"
#include "altitude.h"
#include "PID.h"
#include "bsp_dma.h"
INT8U OS_ERROR;







OS_CPU_SR  cpu_sr = 0u;
int m=0;
int time=0;

#define GY86_THREAD    7
#define GY86_SIZE      256
OS_STK GETDATA_thread_stack[GY86_SIZE];

#define Control_Motor  8
#define Control_SIZE   128
OS_STK Control_thread_stack[Control_SIZE];


#define ANO_THREAD   	 9
#define ANO_SIZE     	 128
OS_STK ANO_thread_stack[ANO_SIZE];
/*
static void change(float* a,char b,char c){
	*a=b*1.0f;
	while(c!=0){
		(*a)=(*a)/10;
		c--;
	}
}
*/

void ANO_Thread(void *a){
	a=a;
	while(1){
		ANO_Quant_Transform((short)(q0*10000),(short)(q1*10000),(short)(q2*10000),(short)(q3*10000));
		ANO_Angle_Transform((short)(angle.roll*100),(short)(angle.pitch*100),(short)(angle.yaw*100));
		ANO_PWN_Transform();
		ANO_IMU_Transform((short)(mpu.ACC_X*100),(short)(mpu.ACC_Y*100),(short)(mpu.ACC_Z*100),(short)(mpu.ACC_X)*100,(short)(mpu.GYR_Y*100),(short)(mpu.GYR_Z*100));
		ANO_HMC_Transform((short)(hmc.X),(short)(hmc.Y),(short)(hmc.Z));	
		ANO_PID_Transform((short)(pitchShell.Kp),(short)(pitchShell.Ki),(short)(pitchShell.Kd));
	}
}
/*
在所有外设已经调整完成之后，开始写所使用到的引脚
E 接收 使用的是 TIM_CH4 也就是 GPIOA 11
雷达使用PA9 和PA10进行
*/

void GY86_Thread(void* a ){
	a=a;
	static float END_TIME,START_TIME;
	while(1){
	GY86_getdata();
	END_TIME=GY86_GETTIME();
	MadgwickAHRSupdate(mpu.GYR_X, mpu.GYR_Y, mpu.GYR_Z, mpu.ACC_X, mpu.ACC_Y,mpu.ACC_Z, hmc.X, hmc.Y, hmc.Z,END_TIME-START_TIME);
	START_TIME=GY86_GETTIME();
	OSTimeDly(1);
	}
}

void ChangeMotor_Thread(void* a){
	a=a;
	static float END_TIME,START_TIME;
	while(1){
		if(flag==1){
			pitchShell.Kp=PID[0];
			pitchShell.Ki=PID[1];
			pitchShell.Kd=PID[2];
			flag=0;
		}
		ChangeToExpert();//角度的转换
		END_TIME=PID_GETTIME();
		Shell_PID_Pitch(END_TIME-START_TIME);
		START_TIME=PID_GETTIME();
		OS_ENTER_CRITICAL();
		
		Power_Change();
		pitch_Change();
		ChangeMotor();
		
		OS_EXIT_CRITICAL();
		OSTimeDly(1);
	}
}

void MY_Init(void){	
	TIM1_CONFIG();	
	TIM4_CONFIG();
	TIM3_CONFIG();
	//PPM_Init();    //先初始电调

	I2C1_CONFIG();
	I2C_DeInit(I2C3);
	I2C3_CONFIG();
	OLED_Init();
	LED_CONFIG();
	//Buttons_CONFIG();
	USART1_CONFIG(); //给雷达使用
	
	USART6_CONFIG(); //给蓝牙使用
	USART6_DMA_CONFIG();
	MPU_Init();
	HMC_Init();
	PID_Init();
	GY86_offset(8000);
	Quat_Init();
  //HMC_offset();
	OLED_PRINT("let's go",2);
	//Fly_Start();
	//OLED_PRINT("let's go",2);
	
	TIM2_CONFIG();
	TIM5_CONFIG();
}

int main(){
	MY_Init();
	SysTick_Config(SystemCoreClock/1000);
	OS_TRACE_INIT(); 
	OS_TRACE_START();  
	OSInit(); 
	OSTaskCreate(GY86_Thread,(void *)0,(OS_STK *)&GETDATA_thread_stack[GY86_SIZE-1],GY86_THREAD);
	OSTaskCreate(ChangeMotor_Thread,(void *)0,(OS_STK *)&Control_thread_stack[Control_SIZE-1],Control_Motor);
	OSTaskCreate(ANO_Thread,(void *)0,(OS_STK *)&ANO_thread_stack[ANO_SIZE-1],ANO_THREAD );
	//OSTaskNameSet(7,(uint8_t*)"GY86",&OS_ERROR);
	//OSTaskNameSet(8,(uint8_t*)"ChangeMotor",&OS_ERROR);	
	//OSTaskNameSet(9,(uint8_t*)"ANO",&OS_ERROR);
	OSStart();
}




