#include "bsp_oled.h"
#include "bsp_zifu.h"
#include <stdio.h>
#include "bsp_systick.h"
void SEND_BYTE(u8 addr,u8 data){
	//���Ȼ�����Ҫi2c ���߲���æµ
	while(I2C_GetFlagStatus(OLED_I2C,I2C_FLAG_BUSY)==1);
	//������ʼ�ź�
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
	

void OLED_SetPos(unsigned char x, unsigned char y) //������ʼ������
{ 
	char str1[4];
	str1[0]=0xb0+y;
	str1[1]=((x&0xf0)>>4)|0x10;
	str1[2]=(x&0x0f)|0x01;
	str1[3]='\0';
	Send_Str(0x00,str1);//����ָ��
}

void Send_Str(u8 addr,char str1[]){
	int i=0;
	while(I2C_GetFlagStatus(OLED_I2C,I2C_FLAG_BUSY)==1);//��Ҫ��֤��busy
	
	printf(" ����æ\r\n");
	
	I2C_GenerateSTART(OLED_I2C,ENABLE);
	
	while(I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_MODE_SELECT)==ERROR){
//		I2C_GenerateSTART(OLED_I2C,ENABLE);
		
	};

	
	
	I2C_Send7bitAddress(OLED_I2C,OLED_ADDR,I2C_Direction_Transmitter);//���͵�ַ ����ȷ����˭��˭��������

	while(I2C_CheckEvent(OLED_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);		
	I2C_SendData(OLED_I2C,addr);
	while(I2C_CheckEvent(OLED_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	printf("���͵�ַ�źųɹ�\r\n");
	while(str1[i]!='\0'){
		I2C_SendData(OLED_I2C,str1[i]);
		i++;
	
		while(I2C_CheckEvent(OLED_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
		printf("�������ݳɹ�\r\n");
	}	
	I2C_GenerateSTOP(OLED_I2C,ENABLE);
	printf("������Ϣ���\r\n");
}





void Convert_Str(unsigned char x,unsigned char y,unsigned char ch[],unsigned char TextSize){
	unsigned char c=0,i=0,j=0;
	char str[100];
	switch(TextSize){
		case 1:
		{
			while(ch[j]!='\0'){ //�ж� ch �ַ����Ƿ����
				c=ch[j]-32;
				if(x>126)
					{
					x=0;
					y++;
				}
			OLED_SetPos(x,y);
			for(i=0;i<6;i++)
				str[i]=F6x8[c][i];
				x+=6;
				j++;
		}
		str[i]='\0';	
	}
		break;
		case 2:	
		{
		while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					str[i]=(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					str[i+8]=(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
			str[i+8]='\0';
		}
		break;
	}
	Send_Str(0x40,str);
	
}


void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					SEND_BYTE(0X40,F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					SEND_BYTE(0X40,F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					SEND_BYTE(0X40,F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
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
	a[0]=0x8D;//���õ�ɱ�
	a[1]=0x10;//�رյ�ɱ�
	a[2]=0xAE;//OLED����
	a[3]='\0';
	Send_Str(0x00,a);
}

void OLED_CLS(void)//����
{
	OLED_Fill(0x00);
}

void OLED_ON(void){
	char a[4];
	a[0]=0x8D;//���õ�ɱ�
	a[1]=0x14;//���ѵ�ɱ�
	a[2]=0xAF;//����OLED
	Send_Str(0x00,a);
}

void OLED_Init(void)
{

	Systick_Delay_ms(1000);
	char a[29];
	a[0]=0xAE;
	a[1]=0x20;
	a[2]=0x10;
	a[3]=0xb3;
	a[4]=0xc8;
	a[5]=0x00;
	a[6]=0x10;
	a[7]=0x40;
	a[8]=0x81;
	a[9]=0xff;
	a[10]=0xa1;
	a[11]=0xa6;
	a[12]=0xa8;
	a[13]=0x3f;
	a[14]=0xa4;
	a[15]=0xd3;
	a[16]=0x00;
	a[17]=0xd5;
	a[18]=0xf0;
	a[19]=0xd9;
	a[20]=0x22;
	a[21]=0xda;
	a[22]=0x12;
	a[23]=0xdb;
	a[24]=0x20;
	a[25]=0x8d;
	a[26]=0x14;
	a[27]=0xaf;
	a[28]='\0';
	Send_Str(0x00,a);
}

void OLED_Str(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize){
	 char a[100];

	Send_Str(0x40,a);
}
































