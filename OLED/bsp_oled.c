#include "bsp_oled.h"
#include "bsp_zifu.h"
#include <stdio.h>
#include "bsp_systick.h"
void SEND_BYTE(u8 addr,u8 data){
	//首先还是需要i2c 总线并不忙碌
	while(I2C_GetFlagStatus(OLED_I2C,I2C_FLAG_BUSY)==1);
	//产生起始信号
	I2C_GenerateSTART(OLED_I2C,ENABLE);
	while(I2C_CheckEvent(OLED_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	
	I2C_Send7bitAddress(OLED_I2C,OLED_ADDR,I2C_Direction_Transmitter);
	
	while(I2C_CheckEvent(OLED_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	
	
	I2C_SendData(OLED_I2C,addr);
	while(I2C_CheckEvent(OLED_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	
	
	I2C_SendData(OLED_I2C,data);
	while(I2C_CheckEvent(OLED_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	
	I2C_GenerateSTOP(OLED_I2C,ENABLE);
}
	

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	char str1[4];
	str1[0]=0xb0+y;
	str1[1]=((x&0xf0)>>4)|0x10;
	str1[2]=(x&0x0f)|0x01;
	str1[3]='\0';
	Send_Str(0x00,str1);//传送指令
}




void Send_Str(u8 addr,char str1[]){
	
	int i=0;
	while(I2C_GetFlagStatus(OLED_I2C,I2C_FLAG_BUSY)==1);	
	I2C_GenerateSTART(OLED_I2C,ENABLE);
	
	while(I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_MODE_SELECT)==ERROR){};
	I2C_Send7bitAddress(OLED_I2C,OLED_ADDR,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(OLED_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);		
	I2C_SendData(OLED_I2C,addr);
	while(I2C_CheckEvent(OLED_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	while(str1[i]!='\0'){
		I2C_SendData(OLED_I2C,str1[i]);
		i++;
		while(I2C_CheckEvent(OLED_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	}	
	I2C_GenerateSTOP(OLED_I2C,ENABLE);
}







void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[])
{
	unsigned char c = 0,i = 0,j = 0,h=0;
	while(ch[j] != '\0'){		
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					SEND_BYTE(0X40,F6x8[c][i]);
				x += 6;
				j++;
				h++;
			}

}


void OLED_Scroll(int START,int END,int DIRECTION){
	SEND_BYTE(0X00,0X2E);//关闭滚动
	SEND_BYTE(0X00,DIRECTION);//水平向左或向右滚动
	SEND_BYTE(0X00,0X00);//虚拟字节
	//相当于设计之后整块的OLED都会 起始页表示第几页 0--7 中的运行
	SEND_BYTE(0X00,START);//起始页0
	SEND_BYTE(0X00,0X07);//滚动时间间隔
	SEND_BYTE(0X00,END);//终止页 7
	//这相当于是固定的
	SEND_BYTE(0X00,0X00);//虚拟字节
	SEND_BYTE(0X00,0XFF);//虚拟字节
	SEND_BYTE(0X00,0X2F);
}



void OLED_Fill(unsigned char fill_data){
	unsigned char m,n;
	char a[4];
	//char b[128]={0xff};
	for(m=0;m<8;m++){
			a[0]=0xb0+m;
			a[1]=0x00;
			a[2]=0x10;
			a[3]='\0';
			Send_Str(0x00,a);
		for(n=0;n<128;n++)
			{
				SEND_BYTE(0x40,fill_data);
	
			}
	}
}
	
void OLED_OFF(void){
	char a[4];
	a[0]=0x8D;//设置电荷泵
	a[1]=0x10;//关闭电荷泵
	a[2]=0xAE;//OLED休眠
	a[3]='\0';
	Send_Str(0x00,a);
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

void OLED_ON(void){
	char a[4];
	a[0]=0x8D;//设置电荷泵
	a[1]=0x14;//唤醒电荷泵
	a[2]=0xAF;//唤醒OLED
	Send_Str(0x00,a);
}

void OLED_Init(void)
{
	Systick_Delay_ms(1000);
	char a[29];
	a[0]=0x20; //连续发送两个 设置是页编辑
	a[1]=0x02;
	a[2]=0x00;
	a[2]='\0';
	Send_Str(0x00,a);
	OLED_ON();
	OLED_Up(0);
	OLED_SetPos(0, 7);
	OLED_Fill(0x00);
}

void OLED_Str(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize){
	 char a[100];
	Send_Str(0x40,a);
}



void OLED_PRINT( char ch[]){
	(unsigned char)ch;
	static int line=7;
	int length=0;
	unsigned char c = 0,i = 0,j = 0;
	
	while(ch[j] != '\0'){		
		if(length>120){
			length=0;
			line=(line+1)%8;
			OLED_Up(line*8-1);
			OLED_SetPos(0,line%8);
			
		}
		c = ch[j] - 32;		
		for(i=0;i<6;i++)
			SEND_BYTE(0X40,F6x8[c][i]);
			j++;
			length+=6;
		}
	line=(line+1)%8;
	OLED_Up(line*8-1);
	OLED_SetPos(0,line%8);
}


void OLED_Up(uint8_t up){
	static char a[]={0xD3,0x00};
	a[1]=up;
	Send_Str(0x00,a);
}


























