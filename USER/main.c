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

int m=0;
int time=0;

int main(){

	LED_CONFIG();
	USART6_CONFIG();
	I2C1_CONFIG();	
	I2C_DeInit(I2C3);
	I2C3_CONFIG();
	TIM1_CONFIG();
	TIM3_CONFIG();
	OLED_Init();
	Fly_Start();
	LED_ON();
	while(1){
		Show_PPM();
		CH1_Change(TIM1_DataBuf[0]);
		CH2_Change(TIM1_DataBuf[1]);
		CH3_Change(TIM1_DataBuf[2]);
		CH4_Change(TIM1_DataBuf[3]);
		
	}

}





    

  