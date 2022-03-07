#ifndef _BSP_OLED_H
#define _BSP_OLED_H
#include "bsp_i2c.h"
#include "bsp_tim.h"

#define OLED_ADDR 0x78
#define OLED_I2C  I2C1







void Send_Str(uint8_t addr,char str1[]);
void OLED_SetPos(unsigned char x, unsigned char y);
void Convert_Str(unsigned char x,unsigned char y,unsigned char ch[],unsigned char TextSize);

void OLED_Fill(unsigned char fill_data);
void SEND_BYTE(uint8_t addr,uint8_t data);
void OLED_OFF(void);
void OLED_ON(void);
void OLED_Init(void);
void OLED_CLS(void);void OLED_Str(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void SEND_BYTE(uint8_t addr,uint8_t data);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
#endif


