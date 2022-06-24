#ifndef _BSP_OLED_H
#define _BSP_OLED_H
#include "bsp_i2c.h"
#include "bsp_tim.h"
#include <stdio.h>

#define OLED_ADDR 0x78
#define OLED_I2C  I2C1
#define Scroll_Right 0x26
#define Scroll_Left  0x27
#define Page0    0x00
#define Page1    0x01
#define Page2    0x02
#define Page3    0x03
#define Page4    0x04
#define Page5    0x05
#define Page6    0x06
#define Page7    0x07

void Send_Str(uint8_t addr,char str1[]);
void OLED_SetPos(unsigned char x, unsigned char y);

void OLED_Fill(unsigned char fill_data);
void SEND_BYTE(uint8_t addr,uint8_t data);
void OLED_OFF(void);
void OLED_ON(void);
void OLED_Init(void);
void OLED_CLS(void);void OLED_Str(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void SEND_BYTE(uint8_t addr,uint8_t data);
void OLED_ShowStr(unsigned char x, unsigned char y,  char ch[]);
void OLED_Scroll(int START,int END,int DIRECT); //∆¡ƒªœÚ…œ“∆∂Ø
void OLED_Up(uint8_t up);
void OLED_PRINT(  char ch[]);
#endif


