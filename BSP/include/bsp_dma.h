#ifndef _BSP_DMA_H
#define _BSP_DMA_H

#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "bsp_usart.h"

extern uint8_t sendbuf[1024];
extern uint8_t receivebuf[1024];
extern short buffersize;

void DMA2_Stream7_IRQHandler(void);

void USART6_DMA_CONFIG(void);

void Bluetooth_senddata(void* buffer,uint16_t size);

#endif




