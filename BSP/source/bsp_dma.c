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
	//����DMAʱ���ж�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	   /*DMA�����ж�����*/
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	
	//DMA2�� Channel 5 Stream 2����������
	DMA_DeInit(DMA2_Stream2);
	DMA_INIT.DMA_PeripheralBaseAddr=(uint32_t)&USART6->DR; //���������ַ
	DMA_INIT.DMA_Channel=DMA_Channel_5; //DMA���� ͨ��
	DMA_INIT.DMA_Memory0BaseAddr=(uint32_t)receivebuf;//���ô洢����ַ
	DMA_INIT.DMA_DIR=DMA_DIR_PeripheralToMemory;//���ô��䷽��
	DMA_INIT.DMA_BufferSize=(uint32_t)buffersize;//�����С
	DMA_INIT.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ����
	DMA_INIT.DMA_MemoryInc=DMA_MemoryInc_Enable;//�洢���ĵ�ַ������
	DMA_INIT.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;//�����ַ���ݵ�λ
	DMA_INIT.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;//memory ��ַ���ݵ�λ
	DMA_INIT.DMA_Mode=DMA_Mode_Normal;//DMAģʽ������ģʽ
	DMA_INIT.DMA_Priority=DMA_Priority_High;//���ȼ���
	DMA_INIT.DMA_FIFOMode=DMA_FIFOMode_Disable;//FIFOģʽ��ʹ��
	DMA_INIT.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;//FIFO��ֵѡ��
	DMA_INIT.DMA_MemoryBurst=DMA_MemoryBurst_Single;//�洢��ͻ��ģʽѡ��
	DMA_INIT.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;//����ͻ��ģʽѡ��
	DMA_Init(DMA2_Stream2,&DMA_INIT);
	
	
	DMA_Cmd(DMA2_Stream2,ENABLE);
	
	//���ô��ڵķ���
	DMA_DeInit(DMA2_Stream7);
	DMA_INIT.DMA_PeripheralBaseAddr=(uint32_t)&USART6->DR;
	DMA_INIT.DMA_Memory0BaseAddr=(uint32_t)sendbuf;
	DMA_INIT.DMA_DIR=DMA_DIR_MemoryToPeripheral;
	DMA_INIT.DMA_BufferSize=0;//��ʱû��������Ҫ����
	
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
	
	DMA_Init(DMA2_Stream7,&DMA_INIT);
	
	
	
	USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);//ʹ��DMA���ڷ�������	
}


void Bluetooth_senddata(void* buffer,uint16_t size){
	if(!size) return ;
	while(DMA_GetCurrDataCounter(DMA2_Stream7));
	if(buffer) memcpy(sendbuf,buffer,(size>1024?1024:size));
	//��Ϊ���жϹ����У��ر���DMA2_Stream7
	DMA2_Stream7->NDTR=size;
	DMA_Cmd(DMA2_Stream7,ENABLE);
}



