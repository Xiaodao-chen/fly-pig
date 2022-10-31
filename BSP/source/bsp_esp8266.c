#include "bsp_esp8266.h"
#include "bsp_led.h"
#include <string.h>
#include <stdlib.h>
#include<stdio.h>
#include "bsp_oled.h"
#include "bsp_systick.h"
#include "ucos_ii.h"


char esp[50];

extern int m;
/*
由于ESP8266 的输出会有问题，所以打算以后使用蓝牙进行通信，但是串口不会改变，依然是使用USART6 
*/

void esp8266_config(){
	Usart_SendStr(USART6,  "AT+CWMODE=1\r\n",sizeof( "AT+CWMODE=1\r\n"));
}

// 将USART_IRQHandler 数据接收
//void USART6_IRQHandler(){
//	static int num=0;
//	if(USART6->SR & USART_SR_RXNE &&num<100){	
//		esp[num++]=USART6->DR;
//		
//	}
//	if(USART6->SR & USART_SR_IDLE){
//		//USART6->DR;
//		num=0;
//	}
//}

	void USART6_IRQHandler(){
		static int m=0;
		if(USART_GetITStatus(USART6,USART_IT_RXNE)!=RESET){
			esp[m]=USART_ReceiveData(USART6);
			m++;
		}else if(USART_GetITStatus(USART6,USART_IT_IDLE)!=RESET){
			//表示接受完毕
			USART6->SR;
			USART6->DR;//清除中断
			m=0;
		
		}

	}

void ESP8266_TCP_CONNECT(){
	//printf("AT+RST\r\n");  //若是新的esp8266则需要开启一次
	printf("AT+CWMODE=1\r\n");
	//Systick_Delay_ms(5);
	//OLED_PRINT((char*)esp,2);
	printf("AT+CWJAP=\"fly-pig\",\"uestc404\"\r\n");
	//Systick_Delay_ms(5);
	while(strstr(esp,"OK")==NULL){
	//	Systick_Delay_ms(5);
		printf("AT+CWJAP=\"fly-pig\",\"uestc404\"\r\n");
	}
	//OLED_PRINT("WIFI CONNECT",2);
	memset(esp,0,sizeof(esp));
	printf("AT+CIPMUX=0\r\n");
	//OLED_PRINT(esp,2);
	memset(esp,0,sizeof(esp));
	printf("AT+CIPSTART=\"TCP\",\"192.168.4.1\",9877\r\n");
	while( strstr(esp,"OK")==NULL){
		//Systick_Delay_ms(100);
		printf("AT+CIPSTART=\"TCP\",\"192.168.4.1\",9877\r\n");
	}
	//Systick_Delay_ms(5);
	OLED_Fill(0x00);
	//Systick_Delay_ms(500);
	printf("AT+CIPMODE=1\r\n");
	//Systick_Delay_ms(50);
	printf("AT+CIPSEND\r\n");
	//OLED_PRINT("TCP CONNECT",2);
	memset(&esp,0,sizeof(esp));
}


void ESP8266_ServerSet(void){
	//printf("AT+RST\r\n");  //若是新的esp8266则需要开启一次
	printf("AT+CWMODE=3\r\n");
	//printf("AT+RST\r\n");
	//Systick_Delay_ms(5);
	printf("AT+CWSAP=\"fly_pig\",\"uestc404\",1,3\r\n");
	//Systick_Delay_ms(5);
	while(strstr(esp,"OK")==NULL){
		//Systick_Delay_ms(5);
		printf("AT+CWSAP=\"fly_pig\",\"uestc404\",1,3\r\n");
	}
	//OLED_PRINT("WIFI SET",2);
	memset(esp,0,sizeof(esp));
	printf("AT+CIPMUX=1\r\n");
	memset(esp,0,sizeof(esp));
	printf("AT+CIPSERVER=1,9987\r\n");
	
	while( strstr(esp,"OK")==NULL){
	//	Systick_Delay_ms(100);
		printf("AT+CIPSERVER=1,9987\r\n");
	}
	//Systick_Delay_ms(5);
	//OLED_PRINT("TCP SET",2);
	memset(&esp,0,sizeof(esp));
}

void ESP_Receive(void){
	//OLED_PRINT(esp+11,2);
	memset(&esp,0,sizeof(esp));
}


void ESP_Send(char* message){
	int length=strlen(message);
	//printf(message);
	char m[15];
	sprintf(m,"AT+CIPSEND=0,%d",length);
	printf(m);
//	Systick_Delay_ms(10);
	printf(message);
	//memset(&esp,0,sizeof(esp));
}

