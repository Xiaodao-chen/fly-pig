#include "bsp_led.h"
#include "bsp_tim.h"
#include "bsp_systick.h"
void LED_CONFIG(void){
	//≈‰÷√GPIOA
	GPIO_InitTypeDef gpio_struct;
		//ø™∆Ù ±÷”£ª
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	gpio_struct.GPIO_Pin=GPIO_Pin_5;
	gpio_struct.GPIO_Speed=GPIO_Speed_100MHz;
	gpio_struct.GPIO_OType=GPIO_OType_PP;  
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Init(GPIOA,&gpio_struct);
}

void LED_ON(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
}

void LED_OFF(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}


void LED_SHINING(int ti){
	while(1){
	
	LED_OFF();
	Systick_Delay_ms(1000);
	LED_ON();
	Systick_Delay_ms(1000);
	}
}

