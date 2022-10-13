#ifndef _BSP_RAY_H
#define _BSP_RAY_H
#include "stm32f4xx.h"
#include "bsp_i2c.h"


#define TF_I2C I2C1
#define TF_ADD_Write 0X20
#define TF_ADD_Read  0X21
#define I2C_MODE   0
#define USART_MODE 1

void Get_Hight(void);
void HEIGHT_SHOW(void);
void TF_Init(void);
void Set_TF(uint8_t register_h,uint8_t register_l ,uint8_t command);
void TF_Commands(uint8_t* commands,int length);
#endif

