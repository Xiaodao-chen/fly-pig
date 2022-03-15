#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_tim.h"
#include "bsp_oled.h"
#include "bsp_systick.h"
#include <stdio.h>
#include "bsp_esp8266.h"


unsigned char a[50];
int m=0;
int time=0;
	int main(){
	TIM4_Init();
	LED_CONFIG();
	USART1_CONFIG();
	USART6_CONFIG();
	I2C_CONFIG();	
	LED_ON();
	OLED_Init();

	OLED_ON();
	Systick_Delay_ms(100);
	OLED_SetPos(0, 0);
	OLED_Fill(0x00);	
	printf("hello\r\n");
	Systick_Delay_ms(1000);
	printf("what fuck");
		LED_SHINING(1000);
}



