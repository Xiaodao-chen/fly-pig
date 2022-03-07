#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_tim.h"
#include "bsp_oled.h"
#include "bsp_systick.h"
#include <stdio.h>
#include "bsp_esp8266.h"

void I2C_Addr_Detect(void);

char a[20];
int time=0;
	int main(){
	TIM4_Init();
	LED_CONFIG();
	USART1_CONFIG();
	USART6_CONFIG();
	I2C_CONFIG();	
	LED_ON();
	OLED_Init();
	printf("success init\r\n");	
	OLED_ON();
	Systick_Delay_ms(1000);
	OLED_Fill(0xFF);
	Systick_Delay_ms(1000);	
	OLED_Fill(0X00);
	Systick_Delay_ms(1000);
	//OLED_Str(0,3,(unsigned char*)"hello world",1);
	Convert_Str(0,3,(unsigned char*)"hello world",1);
	OLED_ShowStr(0,3,(unsigned char*)"hello world",1);  
	printf("finish");

	Usart_SendStr(USART6,"AT\r\n");
	Systick_Delay_ms(500);
	printf("%s",a);
	Usart_SendStr(USART6,"AT+CWMODE=1\r\n");
	Systick_Delay_ms	(1000);
	Usart_SendStr(USART6,"AT+CWJAP=\"cxd\",\"1234567890\"\r\n");

	LED_SHINING(1000);
}


