#include "bsp_esp8266.h"
#include "bsp_led.h"
extern char a[30];
void esp8266_config(){
	Usart_SendStr(USART6,  "AT+CWMODE=1\r\n");
}


void USART6_IRQHandler(){
	static	int num=0;
	while(USART_GetITStatus(USART6,USART_IT_RXNE)!=RESET && num<19){
		a[num++]=USART_ReceiveData(USART6);
	}
	if(USART_GetFlagStatus(USART6,USART_IT_IDLE)!=RESET && num<19){
	a[num]='\0';
	LED_OFF();
	num=0;
	}  
}


