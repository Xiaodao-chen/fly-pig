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
void I2C_Send_Str(I2C_TypeDef * i2c,uint8_t i2c_addr,uint8_t addr,uint8_t* data ,uint8_t length);
void I2C_Send_Byte(I2C_TypeDef * i2c,uint8_t i2c_addr ,uint8_t addr,uint8_t data);
void MY_I2C_Send(I2C_TypeDef* i2c,uint8_t Address, uint32_t data, uint8_t size);
void MY_I2C_Receive(I2C_TypeDef* i2c,uint8_t Address, uint8_t Register, uint8_t *ReceiveBuff, uint8_t size);
void Receive_Str(I2C_TypeDef* i2c,uint8_t i2c_addr,uint8_t Register,uint8_t* Receive,uint8_t size);
void I2C_Receive(I2C_TypeDef* i2c,uint8_t Address, uint8_t Register, uint8_t *ReceiveBuff, uint8_t size);
void I2C_Send(I2C_TypeDef* i2c,uint8_t Address, uint8_t *SendBuff, uint8_t size);

void Lib_I2C_Receive(I2C_TypeDef* i2c,uint8_t Address, uint8_t Register, uint8_t *ReceiveBuff, uint8_t size);

#endif





