#ifndef _BSP_ESP8266_H
#define _BSP_ESP8266_H
#include "bsp_usart.h"



void esp8266_config(void);
void ESP_Receive(void);
void ESP8266_TCP_CONNECT(void);
void ESP_Send(char* message);
void ESP8266_ServerSet(void);
#endif

