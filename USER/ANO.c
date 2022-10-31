#include "ANO.h"
#include "stm32f4xx.h"
#include "PID.h"
#include "bsp_usart.h"
#include "bsp_ppm.h"
#include "datafusion.h"
#include "bsp_dma.h"
char send_buffer[20];

void ANO_Angle_Transform(short roll, short pitch, short  yaw)
{
  uint8_t _cnt = 0;

  send_buffer[_cnt++] = 0xAA;
  send_buffer[_cnt++] = 0xff;
  send_buffer[_cnt++] = 0x03;
  send_buffer[_cnt++] = 0;

  //roll
  send_buffer[_cnt++] = BYTE0(roll);
  send_buffer[_cnt++] = BYTE1(roll);
  //pitch
  send_buffer[_cnt++] = BYTE0(pitch);
  send_buffer[_cnt++] = BYTE1(pitch);
  //yaw
  send_buffer[_cnt++] = BYTE0(yaw);
  send_buffer[_cnt++] = BYTE1(yaw);
  //state
  send_buffer[_cnt++] = 0x01;
  //length
  send_buffer[3] = _cnt - 4;
  //data check
  uint8_t check_sum1 = 0, check_sum2 = 0;
  for (uint8_t i = 0; i < _cnt; i++)
  {
    check_sum1 += send_buffer[i];
    check_sum2 += check_sum1;
  }
  send_buffer[_cnt++] = check_sum1;
  send_buffer[_cnt++] = check_sum2;
	//Usart_SendStr(USART6,send_buffer,_cnt);
	Bluetooth_senddata(send_buffer,_cnt);
}

void ANO_HMC_Transform(short MX,short MY,short MZ){
	uint8_t _cnt=0;
	send_buffer[_cnt++] = 0xAA;
  send_buffer[_cnt++] = 0xff;
  send_buffer[_cnt++] = 0x02;
  send_buffer[_cnt++] = 0x00;
	
	send_buffer[_cnt++] = BYTE0(MX);
  send_buffer[_cnt++] = BYTE1(MX);	
	
	
	send_buffer[_cnt++] = BYTE0(MY);
  send_buffer[_cnt++] = BYTE1(MY);	
	
	send_buffer[_cnt++] = BYTE0(MZ);
  send_buffer[_cnt++] = BYTE1(MZ);	
	
	//alt_bar
	send_buffer[_cnt++] = 0;
  send_buffer[_cnt++] = 0;	
	send_buffer[_cnt++] = 0;
  send_buffer[_cnt++] = 0;	
	//tmp
	send_buffer[_cnt++] = 0;
  send_buffer[_cnt++] = 0;	
	send_buffer[_cnt++] = 0;
  send_buffer[_cnt++] = 0;	
	
	
	send_buffer[3] = _cnt - 4;
	
  uint8_t check_sum1 = 0, check_sum2 = 0;
  for (uint8_t i = 0; i < _cnt; i++)
  {
    check_sum1 += send_buffer[i];
    check_sum2 += check_sum1;
  }
  send_buffer[_cnt++] = check_sum1;
  send_buffer[_cnt++] = check_sum2;
	//Usart_SendStr(USART6,send_buffer,_cnt);
	Bluetooth_senddata(send_buffer,_cnt);
}

void ANO_IMU_Transform(short AX,short AY,short AZ,short GX,short GY,short GZ){
	uint8_t _cnt=0;
	send_buffer[_cnt++] = 0xAA;
  send_buffer[_cnt++] = 0xff;
  send_buffer[_cnt++] = 0xF1;
  send_buffer[_cnt++] = 0x00;
	
	send_buffer[_cnt++] = BYTE0(AX);
  send_buffer[_cnt++] = BYTE1(AX);	
	
	
	send_buffer[_cnt++] = BYTE0(AY);
  send_buffer[_cnt++] = BYTE1(AY);	
	
	send_buffer[_cnt++] = BYTE0(AZ);
  send_buffer[_cnt++] = BYTE1(AZ);	
	
	send_buffer[_cnt++] = BYTE0(GX);
  send_buffer[_cnt++] = BYTE1(GX);	
	
	send_buffer[_cnt++] = BYTE0(GY);
  send_buffer[_cnt++] = BYTE1(GY);	
	
	send_buffer[_cnt++] = BYTE0(GZ);
  send_buffer[_cnt++] = BYTE1(GZ);	
	
	send_buffer[_cnt++] = 0x01;
	//length
	send_buffer[3] = _cnt - 4;
 
	
	uint8_t check_sum1 = 0, check_sum2 = 0;
  for (uint8_t i = 0; i < _cnt; i++)
  {
    check_sum1 += send_buffer[i];
    check_sum2 += check_sum1;
  }
  send_buffer[_cnt++] = check_sum1;
  send_buffer[_cnt++] = check_sum2;
	//Usart_SendStr(USART6,send_buffer,_cnt);
	Bluetooth_senddata(send_buffer,_cnt);
}


/*传入接收到的油门值

buf[0] 是左右方向
buf[1] 是前后方向
buf[2] 是油门方向
buf[3] 是左旋转和右旋转方向
*/
void ANO_PWN_Transform(void){
	uint8_t _cnt=0;
	send_buffer[_cnt++] = 0xAA;
  send_buffer[_cnt++] = 0xff;
  send_buffer[_cnt++] = 0x20;
  send_buffer[_cnt++] = 0x00;
	//
	send_buffer[_cnt++] = BYTE0(TIM1_DataBuf[0]);
  send_buffer[_cnt++] = BYTE1(TIM1_DataBuf[0]);	
	
	send_buffer[_cnt++] = BYTE0(TIM1_DataBuf[1]);
  send_buffer[_cnt++] = BYTE1(TIM1_DataBuf[1]);	
	
	send_buffer[_cnt++] = BYTE0(TIM1_DataBuf[2]);
  send_buffer[_cnt++] = BYTE1(TIM1_DataBuf[2]);	
	
	send_buffer[_cnt++] = BYTE0(TIM1_DataBuf[3]);
  send_buffer[_cnt++] = BYTE1(TIM1_DataBuf[3]);

	send_buffer[3] = _cnt - 4;
	uint8_t check_sum1 = 0, check_sum2 = 0;
  for (uint8_t i = 0; i < _cnt; i++)
  {
    check_sum1 += send_buffer[i];
    check_sum2 += check_sum1;
  }
  send_buffer[_cnt++] = check_sum1;
  send_buffer[_cnt++] = check_sum2;
	//Usart_SendStr(USART6,send_buffer,_cnt);
	Bluetooth_senddata(send_buffer,_cnt);
}


void ANO_PID_Transform(short P,short I,short D){
	uint8_t _cnt=0;
	send_buffer[_cnt++] = 0xAA;
  send_buffer[_cnt++] = 0xff;
  send_buffer[_cnt++] = 0xF2;
  send_buffer[_cnt++] = 0x00;
	
	send_buffer[_cnt++] = BYTE0(P);
  send_buffer[_cnt++] = BYTE1(P);	
	
	
	send_buffer[_cnt++] = BYTE0(I);
  send_buffer[_cnt++] = BYTE1(I);	
	
	send_buffer[_cnt++] = BYTE0(D);
  send_buffer[_cnt++] = BYTE1(D);	
	
	
	send_buffer[3] = _cnt - 4;
	
  uint8_t check_sum1 = 0, check_sum2 = 0;
  for (uint8_t i = 0; i < _cnt; i++)
  {
    check_sum1 += send_buffer[i];
    check_sum2 += check_sum1;
  }
  send_buffer[_cnt++] = check_sum1;
  send_buffer[_cnt++] = check_sum2;
	//Usart_SendStr(USART6,send_buffer,_cnt);
	Bluetooth_senddata(send_buffer,_cnt);
}

void ANO_Quant_Transform(short q0,short q1, short q2,short q3){
	uint8_t _cnt=0;
	send_buffer[_cnt++] = 0xAA;
  send_buffer[_cnt++] = 0xff;
  send_buffer[_cnt++] = 0x04;//四元数格式 
  send_buffer[_cnt++] = 0x00;
	
	send_buffer[_cnt++]=BYTE0(q0);
	send_buffer[_cnt++]=BYTE1(q0);
	
	send_buffer[_cnt++]=BYTE0(q1);
	send_buffer[_cnt++]=BYTE1(q1);
	
	send_buffer[_cnt++]=BYTE0(q2);
	send_buffer[_cnt++]=BYTE1(q2);
	
	send_buffer[_cnt++]=BYTE0(q3);
	send_buffer[_cnt++]=BYTE1(q3);
	
	send_buffer[_cnt++]=0x00;
	
	send_buffer[3] = _cnt - 4;
	
  uint8_t check_sum1 = 0, check_sum2 = 0;
  for (uint8_t i = 0; i < _cnt; i++)
  {
    check_sum1 += send_buffer[i];
    check_sum2 += check_sum1;
  }
  send_buffer[_cnt++] = check_sum1;
  send_buffer[_cnt++] = check_sum2;
	//Usart_SendStr(USART6,send_buffer,_cnt);
	Bluetooth_senddata(send_buffer,_cnt);
}
