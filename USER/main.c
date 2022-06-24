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
int m=0;
int time=0;


int main(){
	TIM1_CONFIG(); 
	TIM3_CONFIG();
	LED_CONFIG();
	//USART1_CONFIG();
	//由于中断所以卡死了
	USART6_CONFIG();
	I2C1_CONFIG();	
	I2C_DeInit(I2C3);
	I2C3_CONFIG();
	LED_ON();
	OLED_Init();
	LED_OFF();
	//Fly_Start();
	while(1){
		Show_PPM();
	}
	LED_OFF();
}


