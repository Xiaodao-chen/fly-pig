#include "bsp_esp8266.h"
#include "bsp_led.h"
#include <string.h>
#include <stdlib.h>
#include<stdio.h>
#include "bsp_oled.h"
#include "bsp_systick.h"



char esp[100];

extern int m;

void esp8266_config(){
	Usart_SendStr(USART6,  "AT+CWMODE=1\r\n");
}









// 将USART_IRQHandler 数据接收
void USART6_IRQHandler(){
	static int num=0;
	if(USART6->SR & USART_SR_RXNE &&num<100){	
		esp[num++]=USART6->DR;
		
	}
	if(USART6->SR & USART_SR_IDLE){
		num=0;
	}
}

void esp8266_tcp_connect(){
	printf("AT+RST\r\n");
		
	printf("AT+CWMODE=1\r\n");
	Systick_Delay_ms(5);
	OLED_ShowStr(0,3,(char*)esp);
	printf("AT+CWJAP=\"fly-pig\",\"uestc404\"\r\n");
	Systick_Delay_ms(5);
	OLED_ShowStr(0,5,(char*)esp);
	printf("AT+CIPSTART=\"TCP\",\"192.168.4.1\",333\r\n");
	Systick_Delay_ms(5);
	OLED_ShowStr(0,4,( char*)esp);	

	Systick_Delay_ms(5);

	printf("AT+CIPSEND\r\n");
	Systick_Delay_ms(5);
	OLED_Fill(0x00);
	OLED_ShowStr(0,1,(char*)esp);
}






void StrReplaceB(char* strSrc, char* strFind, char* strReplace)
{
	while (*strSrc != '\0')
	{
		if (*strSrc == *strFind)
		{
			if (strncmp(strSrc, strFind, strlen(strFind)) == 0)
			{
				int i = strlen(strFind);
				int j = strlen(strReplace);
				char* q = strSrc+i;
				char* p = q;//p、q均指向剩余字符串的首地址
				char* repl = strReplace;
				int lastLen = 0;
				while (*q++ != '\0')
					lastLen++;
				char* temp =(char*)malloc(sizeof(char)*(lastLen+1));//临时开辟一段内存保存剩下的字符串,防止内存覆盖
				for (int k = 0; k < lastLen; k++)
				{
					*(temp+k) = *(p+k);
				}
				*(temp+lastLen) = '\0';
				while (*repl != '\0')
				{
					*strSrc++ = *repl++;
				}
				p = strSrc;
				char* pTemp = temp;//回收动态开辟内存
				while (*pTemp != '\0')
				{
					*p++ = *pTemp++;
				}
				free(temp);
				*p = '\0';
			}
			else
				strSrc++;
		}
		else
			strSrc++;
	}
}

void Set_esp8266_touchuan(void){
	//现在关中断，因为没有意义
	//或者现在开中断 将数据发送给oled 显示；？？
	//NVIC_usart6_close();
	printf("AT\r\n");
	Usart_SendStr(USART6,"AT");
	Systick_Delay_ms(100);
	printf("AT+CWMODE=1\r\n");
}


