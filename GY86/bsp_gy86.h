#ifndef __BSP_GY86_H
#define __BSP_GY86_H
#include "stm32f4xx.h"
#include "bsp_oled.h"
#include "bsp_systick.h"
#include "bsp_i2c.h"
#include <stdio.h>

//5883是 MPU6050的从设备，需要初始化才可以接到I2C3的总线上



#define MPU6050_ADDRESS 0Xd0
#define MS5611_ADDRESS  0XEE
#define HMC5883_ADDRESS   0X3C

#define GY86_I2C I2C3		
#define MPU_I2C  I2C3


void GY86_Init(void);
void G_Send_Byte(uint8_t i2c_addr,uint8_t addr,uint8_t data);   
void G_SendStr(uint8_t i2c_addr,uint8_t addr,char str1[]);
void G_Receive_Byte(uint8_t i2c_addr,uint8_t addr,uint8_t* rec);
void G_Receive_Str(uint8_t i2c_addr,uint8_t addr,uint8_t* rec,int size);
void MPU_Receive_Byte(uint8_t addr,uint8_t* rec);
void MPU_Receive_Str(uint8_t addr,uint8_t* rec,int size);
void MPU_Get(void);
void MPU_Show(void);

#endif



