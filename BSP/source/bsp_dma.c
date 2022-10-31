#include "bsp_dma.h"
#include <string.h>
uint8_t sendbuf[1024];
uint8_t receivebuf[1024];


short buffersize=32;

void  DMA2_Stream7_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7)==SET)
    {
        DMA_ClearFlag(DMA2_Stream7,DMA_IT_TCIF7);        
        DMA_Cmd(DMA2_Stream7, DISABLE);  
    }
}


void USART6_DMA_CONFIG(void){
	
	DMA_InitTypeDef DMA_INIT;
	NVIC_InitTypeDef	NVIC_InitStructure;
	//开启DMA时钟中断
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	   /*DMA发送中断设置*/
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	
	//DMA2的 Channel 5 Stream 2是用作接收
	DMA_DeInit(DMA2_Stream2);
	DMA_INIT.DMA_PeripheralBaseAddr=(uint32_t)&USART6->DR; //配置外设地址
	DMA_INIT.DMA_Channel=DMA_Channel_5; //DMA请求 通道
	DMA_INIT.DMA_Memory0BaseAddr=(uint32_t)receivebuf;//配置存储器地址
	DMA_INIT.DMA_DIR=DMA_DIR_PeripheralToMemory;//配置传输方向
	DMA_INIT.DMA_BufferSize=(uint32_t)buffersize;//传输大小
	DMA_INIT.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不变
	DMA_INIT.DMA_MemoryInc=DMA_MemoryInc_Enable;//存储器的地址自增加
	DMA_INIT.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;//外设地址数据单位
	DMA_INIT.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;//memory 地址数据单位
	DMA_INIT.DMA_Mode=DMA_Mode_Normal;//DMA模式：正常模式
	DMA_INIT.DMA_Priority=DMA_Priority_High;//优先级高
	DMA_INIT.DMA_FIFOMode=DMA_FIFOMode_Disable;//FIFO模式不使能
	DMA_INIT.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;//FIFO阈值选择
	DMA_INIT.DMA_MemoryBurst=DMA_MemoryBurst_Single;//存储器突发模式选择，
	DMA_INIT.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;//外设突发模式选择
	DMA_Init(DMA2_Stream2,&DMA_INIT);
	
	
	DMA_Cmd(DMA2_Stream2,ENABLE);
	
	//配置串口的发送
	DMA_DeInit(DMA2_Stream7);
	DMA_INIT.DMA_PeripheralBaseAddr=(uint32_t)&USART6->DR;
	DMA_INIT.DMA_Memory0BaseAddr=(uint32_t)sendbuf;
	DMA_INIT.DMA_DIR=DMA_DIR_MemoryToPeripheral;
	DMA_INIT.DMA_BufferSize=0;//暂时没有数据需要发送
	
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
	
	DMA_Init(DMA2_Stream7,&DMA_INIT);
	
	
	
	USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);//使能DMA串口发送数据	
}


void Bluetooth_senddata(void* buffer,uint16_t size){
	if(!size) return ;
	while(DMA_GetCurrDataCounter(DMA2_Stream7));
	if(buffer) memcpy(sendbuf,buffer,(size>1024?1024:size));
	//因为在中断过程中，关闭了DMA2_Stream7
	DMA2_Stream7->NDTR=size;
	DMA_Cmd(DMA2_Stream7,ENABLE);
}



