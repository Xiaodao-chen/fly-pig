#ifndef __BSP_GY86_H
#define __BSP_GY86_H
#include "stm32f4xx.h"
#include "bsp_oled.h"
#include "bsp_systick.h"
#include "bsp_i2c.h"
#include <stdio.h>
#include "datafusion.h"

//GY86 需要读取的X轴，Y轴，Z轴加速度，温度，还有X轴旋转的角速度，还有Y轴旋转的角速度，Z轴旋转的角速度，
//所以通过这个可以设计一个结构体来专门存储该数据
typedef struct X1{
	int16_t ACC_X;
	int16_t ACC_Y;
	int16_t ACC_Z;
	uint16_t TEMP;
	int16_t GYR_X;
	int16_t GYR_Y;
	int16_t GYR_Z;
}MPU_Struct;

typedef struct X3{
	float ACC_X;
	float ACC_Y;
	float ACC_Z;
	float TEMP;
	float GYR_X;
	float GYR_Y;
	float GYR_Z;
}mpu_Struct;

typedef struct X2{
	float X;
	float Y;
	float Z;
}HMC_Struct;

typedef struct X4{
	float X;
	float Y;
	float Z;
}hmc_Struct;

//5883是 MPU6050的从设备，需要初始化才可以接到I2C3的总线上

#define GYRO_250DPS 131.072f
#define ACCEL_2G 1673.469f

#define MPU6050_ADDRESS 	0XD0 
#define HMC5883_ADDRESS   0X3C



#define GY86_I2C I2C3		
#define MPU_I2C  I2C3
#define HMC_I2C I2C3

void MPU_Init(void);
void G_Receive_Byte(uint8_t i2c_addr,uint8_t addr,uint8_t* rec);
void MPU_Receive_Byte(uint8_t addr,uint8_t* rec);
void HMC_Init(void);
void MPU_Get(void);
void MPU_Show(void);
void HMC_Show(void);
void HMC_Get(void);
void GY86_getdata(void);
void GY86_offset(void);
void angle_show(void);
#endif



