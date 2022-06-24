#ifndef _BSP_ESP8266_H
#define _BSP_ESP8266_H
#include "bsp_usart.h"



void esp8266_config(void);

void StrReplaceB(char* strSrc, char* strFind, char* strReplace);
void Set_esp8266_touchuan(void);
void esp8266_tcp_connect(void);
#endif

