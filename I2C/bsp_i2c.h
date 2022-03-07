#ifndef _BSP_I2C_H
#define _BSP_I2C_H
#include "stm32f4xx.h"

#define I2C1_SCL_Pin GPIO_Pin_6
#define I2C1_SDA_Pin GPIO_Pin_7
#define I2C_SPEED 400000
#define I2C_OWN_ADDR 0x5f






void I2C_CONFIG(void);

#endif





