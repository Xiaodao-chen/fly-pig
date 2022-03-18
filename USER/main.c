#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_tim.h"
#include "bsp_oled.h"
#include "bsp_systick.h"
#include <stdio.h>
#include "bsp_esp8266.h"


unsigned char a[100];
int m=0;
int time=0;

	int main(){
	char m[]={0x59};
	TIM4_Init();
	LED_CONFIG();
	USART1_CONFIG();
	USART6_CONFIG();
	I2C_CONFIG();	
	LED_ON();
	OLED_Init();

	OLED_PRINT("1");
	OLED_PRINT("2");
	OLED_PRINT("3");
	OLED_PRINT("4");
	OLED_PRINT("5");
	OLED_PRINT("I love you");
	OLED_PRINT("7");
	OLED_PRINT("8");
	OLED_PRINT("9");
	OLED_PRINT("10");
	OLED_PRINT("11");
	OLED_PRINT("12");
	OLED_PRINT("12");
	LED_SHINING(1000);
}







