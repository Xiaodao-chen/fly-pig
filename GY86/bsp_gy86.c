#include "bsp_gy86.h"
uint8_t ch[6]={0};
char show0[14]={0};
char show1[14]={0};
char show2[14]={0};
char show3[14]={0};
char show4[14]={0};
char show5[14]={0};
char show6[14]={0};
char show7[14]={0};

//GY86 需要读取的X轴，Y轴，Z轴加速度，温度，还有X轴旋转的角速度，还有Y轴旋转的角速度，Z轴旋转的角速度，所以通过这个可以设计一个结构体来专门存储该数据
typedef struct X1{
	int16_t ACC_X;
	int16_t ACC_Y;
	int16_t ACC_Z;
	int16_t TEMP;
	int16_t GYR_X;
	int16_t GYR_Y;
	int16_t GYR_Z;
}MPU_Struct;

MPU_Struct mpu;


void GY86_Init(void){
	G_Send_Byte(MPU6050_ADDRESS,0X6B,0x80);
	Systick_Delay_ms(10);
	G_Send_Byte(MPU6050_ADDRESS,0x6B,0x01);
	G_Send_Byte(MPU6050_ADDRESS,0X6A,0X00);
	G_Send_Byte(MPU6050_ADDRESS,0X37,0x02);

}

void G_Send_Byte(uint8_t i2c_addr ,u8 addr,u8 data){
	while(I2C_GetFlagStatus(MPU_I2C,I2C_FLAG_BUSY)==1){};
	I2C_GenerateSTART(MPU_I2C,ENABLE);
	while(I2C_CheckEvent(MPU_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR){};
	I2C_Send7bitAddress(MPU_I2C,i2c_addr ,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(MPU_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR){};
	I2C_SendData(MPU_I2C,addr);
	while(I2C_CheckEvent(MPU_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	I2C_SendData(MPU_I2C,data);
	while(I2C_CheckEvent(MPU_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	I2C_GenerateSTOP(MPU_I2C,ENABLE);
}
	
void G_SendStr(uint8_t i2c_addr ,uint8_t addr,char str1[]){	
	int i=0;
	while(I2C_GetFlagStatus(MPU_I2C,I2C_FLAG_BUSY)==1);	
	I2C_GenerateSTART(MPU_I2C,ENABLE);
	while(I2C_CheckEvent(GY86_I2C, I2C_EVENT_MASTER_MODE_SELECT)==ERROR){};
	I2C_Send7bitAddress(GY86_I2C,i2c_addr,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(GY86_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	I2C_SendData(GY86_I2C,addr);
	while(I2C_CheckEvent(GY86_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR){};	
	while(str1[i]!='\0'){
		I2C_SendData(GY86_I2C,str1[i]);
		i++;
		while(I2C_CheckEvent(GY86_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR){};	
	}	
	I2C_GenerateSTOP(GY86_I2C,ENABLE);
}



void MPU_Receive_Byte(uint8_t addr,uint8_t* rec){
	while(I2C_GetFlagStatus(MPU_I2C,I2C_FLAG_BUSY)==1){};
	I2C_GenerateSTART(MPU_I2C,ENABLE);
	while(I2C_CheckEvent(GY86_I2C, I2C_EVENT_MASTER_MODE_SELECT)==ERROR){};
	I2C_Send7bitAddress(GY86_I2C,MPU6050_ADDRESS,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(GY86_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR){};
	I2C_SendData(GY86_I2C,addr);
	while(I2C_CheckEvent(GY86_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR){};	
	I2C_GenerateSTART(MPU_I2C,ENABLE);
	while(I2C_CheckEvent(GY86_I2C, I2C_EVENT_MASTER_MODE_SELECT)==ERROR){};
	I2C_Send7bitAddress(GY86_I2C,0XD1,I2C_Direction_Receiver);
	while(I2C_CheckEvent(GY86_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==ERROR){};
	Systick_Delay_ms(1);
	*rec=I2C_ReceiveData(GY86_I2C);
	I2C3->CR1&=~(1<<10);
	I2C_GenerateSTOP(GY86_I2C,ENABLE);	
}

void MPU_Receive_Str(uint8_t addr,uint8_t *rec,int size){
	int i=0;
	
	while(I2C_GetFlagStatus(MPU_I2C,I2C_FLAG_BUSY)==1){};
	
	I2C_GenerateSTART(MPU_I2C,ENABLE);
	while(I2C_CheckEvent(GY86_I2C, I2C_EVENT_MASTER_MODE_SELECT)==ERROR){};
	I2C_Send7bitAddress(GY86_I2C,MPU6050_ADDRESS,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(GY86_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR){};
	I2C_SendData(GY86_I2C,addr);
	while(I2C_CheckEvent(GY86_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR){};	
	
	I2C_GenerateSTART(MPU_I2C,ENABLE);
	while(I2C_CheckEvent(GY86_I2C, I2C_EVENT_MASTER_MODE_SELECT)==ERROR){};
	I2C_Send7bitAddress(GY86_I2C,0XD1,I2C_Direction_Receiver);
	while(I2C_CheckEvent(GY86_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==ERROR){};
	
	for(i=0;i<size;i++){
		Systick_Delay_ms(1);
		rec[i]=I2C_ReceiveData(GY86_I2C);
		I2C3->CR1|=(1<<10);
	
	}
	I2C3->CR1&=~(1<<10);
	I2C_GenerateSTOP(GY86_I2C,ENABLE);	
}

void MPU_Get(void){
	MPU_Receive_Byte(0X3B,&ch[0]);
	MPU_Receive_Byte(0X3C,&ch[1]);

	mpu.ACC_X=(ch[0]<<8)|ch[1];
	MPU_Receive_Byte(0X3D,&ch[2]);
	MPU_Receive_Byte(0X3E,&ch[3]);
	mpu.ACC_Y=(ch[2]<<8)|ch[3];
	MPU_Receive_Byte(0X3F,&ch[4]);
	MPU_Receive_Byte(0X40,&ch[5]);
	mpu.ACC_Z=(ch[4]<<8)|ch[5];
	MPU_Receive_Byte(0x41,&ch[0]);
	MPU_Receive_Byte(0x42,&ch[1]);
	mpu.TEMP=(ch[0]<<8)|ch[1];
	MPU_Receive_Byte(0x43,&ch[0]);
	MPU_Receive_Byte(0x44,&ch[1]);
	MPU_Receive_Byte(0x45,&ch[2]);
	MPU_Receive_Byte(0x46,&ch[3]);
	MPU_Receive_Byte(0x47,&ch[4]);
	MPU_Receive_Byte(0x48,&ch[5]);
	mpu.GYR_X=(ch[0]<<8)|ch[1];
	mpu.GYR_Y=(ch[2]<<8)|ch[3];
	mpu.GYR_Z=(ch[4]<<8)|ch[5];
}

void MPU_Show(void){
	sprintf(show0,"AX:%6d",mpu.ACC_X);
	sprintf(show1,"AY:%6d",mpu.ACC_Y);
	sprintf(show2,"AZ:%6d",mpu.ACC_Z);
	sprintf(show3,"GX:%6d",mpu.GYR_X);
	sprintf(show4,"GY:%6d",mpu.GYR_Y);
	sprintf(show5,"GZ:%6d",mpu.GYR_Z);
	sprintf(show6,"TP:%6d",mpu.TEMP);
	OLED_ShowStr(0, 0, show0);
	OLED_ShowStr(0, 1, show1);
	OLED_ShowStr(0, 2, show2);
	OLED_ShowStr(60, 0, show3);
	OLED_ShowStr(60, 1, show4);
	OLED_ShowStr(60, 2, show5);
	OLED_ShowStr(0, 3, show6);	
}



