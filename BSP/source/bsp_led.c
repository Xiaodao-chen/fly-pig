#include "bsp_led.h"
#include "bsp_tim.h"
#include "bsp_systick.h"
#include "ucos_ii.h"
void LED_CONFIG(void){

	GPIO_InitTypeDef gpio_struct;
		//¿ªÆôÊ±ÖÓ£»
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	gpio_struct.GPIO_Pin=GPIO_Pin_5;
	gpio_struct.GPIO_Speed=GPIO_Speed_100MHz;
	gpio_struct.GPIO_OType=GPIO_OType_PP;  
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio_struct);
}

void LED_ON(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
}

void LED_OFF(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}


void LED_SHINING(){
	while(1){
	
	LED_OFF();
	TIMDelay(1000);
	LED_ON();
	TIMDelay(1000);
	}
}

