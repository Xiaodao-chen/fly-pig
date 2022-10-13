#include "bsp_oled.h"
#include "bsp_zifu.h"
#include <stdio.h>
#include "bsp_systick.h"
#include <string.h>
void SEND_BYTE(u8 addr,u8 data){
	I2C_Send_Byte(OLED_I2C,OLED_ADDR,addr,data);
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



/*
addr 表示的是地址，即要传送的addr地址，p是所传的指令
*/
void Send_Str(u8 addr,char str1[]){ 
	uint8_t* p=(uint8_t*)str1;
	int length=strlen(str1);
	I2C_Send_Str(OLED_I2C,OLED_ADDR,addr,p,length);
}




void OLED_ShowStr(unsigned char x, unsigned char y, char ch[])
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
 
	char a[29];
	a[0]=0x20; //连续发送两个 设置是页编辑
	a[1]=0x02;
	a[2]=0x00;
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


void OLED_PRINT( char ch[],int size){
	unsigned char* m=(unsigned char*)ch;
	static int line=7;
	static int line_1=4;
	int length=0;
	unsigned char c = 0,i = 0,j = 0;
	switch(size){
		case 1:
		while(m[j] != '\0'){		
		if(length>120){
			length=0;
			line=(line+1)%8;
			OLED_Up(line*8-1);
			OLED_SetPos(0,line%8);
		}
		c = m[j] - 32;		
		for(i=0;i<6;i++)
			SEND_BYTE(0X40,F6x8[c][i]);
			j++;
			length+=6;
		}
	line=(line+1)%8;
	OLED_Up(line*8-1);
	OLED_SetPos(0,line%8);
		case 2:
			while(m[j] != '\0')
			{
				c = m[j] - 32;
				if(length > 120)
				{
					length = 0;
					line_1=(line_1+2)%8;
					OLED_Up(2);
					OLED_SetPos(0,line_1);
				}
			OLED_SetPos(length,line_1%8);
			for(i=0;i<8;i++)
				SEND_BYTE(0X40,F8X16[c*16+i]);
			OLED_SetPos(length,line_1%8+1);
			for(i=0;i<8;i++)
				SEND_BYTE(0X40,F8X16[c*16+i+8]);
				length += 8;
				j++;
			}
				line_1=(line_1+2)%8;
				OLED_Up(line_1*8-1);
			}
	}

void OLED_Up(uint8_t up){
	static char a[]={0xD3,0x00};
	a[1]=up;
	Send_Str(0x00,a);
}
