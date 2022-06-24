#ifndef _BSP_I2C_H
#define _BSP_I2C_H
#include "stm32f4xx.h"
#include "bsp_systick.h"

/*
	GY86使用的引脚是PA8,PC9
	OLED使用的引脚是PB6,PB7
*/

//use for oled
#define I2C1_SCL_Pin GPIO_Pin_6
#define I2C1_SDA_Pin GPIO_Pin_7


//use for gy86
#define I2C3_SDA_Pin GPIO_Pin_9
#define I2C3_SCL_Pin GPIO_Pin_8



#define I2C_SPEED 400000
#define I2C_OWN_ADDR1 0x5F
#define I2C_OWN_ADDR2 0x5E






void I2C1_CONFIG(void);
void I2C3_CONFIG(void);
#endif





