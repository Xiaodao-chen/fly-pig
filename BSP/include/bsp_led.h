#ifndef _BSP_LED_H
#define _BSP_LED_H
#include "stm32f4xx.h"
   
#define OLED2_GPIO                GPIOA
#define OLED2_GPIO_PIN            GPIO_Pin_5

void LED_CONFIG(void);
void LED_OFF(void);
void LED_ON(void);
void LED_SHINING(void);
#endif

