#include "bsp_gy86.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ucos_ii.h"
#include "bsp_tim.h"
uint8_t ch[14]={0};
uint8_t buffer[14]={0};
uint8_t mpu_init[14]={0x6B,0x80,0x6B,0x01,0x6A,0x00,0x37,0x02};
float axo=0,ayo=0,azo=0,gxo=0,gyo=0,gzo=0;//偏移量
extern Angle_t angle;
char show0[14]={0};
char show1[14]={0};
char show2[14]={0};
char show3[14]={0};
char show4[14]={0};
char show5[14]={0};
char show6[14]={0};
char show7[14]={0};
char show8[14]={0};
char show9[14]={0};
char show10[14]={0};
char show11[14]={0};
char show12[14]={0};

MPU_Struct mpu_r;
HMC_Struct hmc_r;
mpu_Struct mpu;
hmc_Struct hmc;
//问题应该出现在我配置 他为连续输出模式，但是模式没有配置成功
//所以接下来应该换一个方式进行配置，想一想问题在哪




void MPU_Init(void){	
	I2C_Send_Byte(HMC_I2C,MPU6050_ADDRESS,0X6B,0x80);
	TIMDelay(10);
	I2C_Send_Byte(HMC_I2C,MPU6050_ADDRESS,0x6B,0x01);
	I2C_Send_Byte(HMC_I2C,MPU6050_ADDRESS,0X6A,0X00);
	I2C_Send_Byte(HMC_I2C,MPU6050_ADDRESS,0X37,0x02);
	//量程±250°/s，±2g/s2
	I2C_Send_Str(HMC_I2C,MPU6050_ADDRESS,0x19,(uint8_t*)"\x09\x1B\x00\x00",4);
}




void HMC_Init(void){
	//需要延时
	TIMDelay(10);
	I2C_Send_Str(HMC_I2C,HMC5883_ADDRESS,0x00,(uint8_t*)"\x78\x00\x00",3);
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
	while(!(I2C3->SR1 & I2C_SR1_RXNE));	
	*rec=I2C_ReceiveData(GY86_I2C);
	I2C3->CR1&=~I2C_CR1_ACK  ;
	I2C_GenerateSTOP(GY86_I2C,ENABLE);	
}

void MPU_Get(void){
	I2C_Receive(GY86_I2C,MPU6050_ADDRESS,0X3B,ch,14);	
	mpu_r.ACC_X=(short)(ch[0]<<8)|ch[1];
	mpu_r.ACC_Y=(short)(ch[2]<<8)|ch[3];
	mpu_r.ACC_Z=(short)(ch[4]<<8)|ch[5];
	mpu_r.TEMP=(short)(ch[6]<<8)|ch[7];
	mpu_r.GYR_X=(short)(ch[8]<<8)|ch[9];
	mpu_r.GYR_Y=(short)(ch[10]<<8)|ch[11];
	mpu_r.GYR_Z=(short)(ch[12]<<8)|ch[13];

}

void HMC_Get(void){
	I2C_Receive(HMC_I2C,HMC5883_ADDRESS,0X03,buffer,6);
	hmc.X=(short)(buffer[0]<<8)|buffer[1];
	hmc.Y=(short)(buffer[4]<<8)|buffer[5];
	hmc.Z=(short)(buffer[2]<<8)|buffer[3];
	if (hmc.X > 0x7fff)
      hmc.X -= 0xffff;
    if (hmc.Y > 0x7fff)
      hmc.Y -= 0xffff;
    if (hmc.Z > 0x7fff)
      hmc.Z -= 0xffff;
}
/*
AX 表示X轴的加速度
GX 表示X角速度 Stores the most recent X axis gyroscope measurement. 存储X轴陀螺仪的测试结果 ，那存储的结果就是绕X轴的角速度
*/
void MPU_Show(void){
	sprintf(show0,"AX:%2.1f",(float)mpu.ACC_X);
	sprintf(show1,"AY:%2.1f",(float)mpu.ACC_Y);
	sprintf(show2,"AZ:%2.1f",(float)mpu.ACC_Z);
	sprintf(show3,"GX:%2.1f",(float)mpu.GYR_X);
	sprintf(show4,"GY:%2.1f",(float)mpu.GYR_Y);
	sprintf(show5,"GZ:%2.1f",(float)mpu.GYR_Z);
	OLED_ShowStr(0, 7, show0);
	OLED_ShowStr(0, 0, show1);
	OLED_ShowStr(0, 1, show2);
	OLED_ShowStr(60, 7, show3);
	OLED_ShowStr(60, 0, show4);
	OLED_ShowStr(60, 1, show5);
}



void HMC_Show(void){
	sprintf(show7,"MX:%2.1f",hmc.X);
	sprintf(show8,"MY:%2.1f",hmc.Y);
	sprintf(show9,"MZ:%2.1f",hmc.Z);
	OLED_ShowStr(0, 3, show7);
	OLED_ShowStr(0, 4, show8);
	OLED_ShowStr(0, 5, show9);
}


//取均值计算出水平状态下偏移量
void GY86_offset(void){
    float axt=0,ayt=0,azt=0,gxt=0,gyt=0,gzt=0;
    for(int i = 0;i<100;i++){
        GY86_getdata();
        axt+=mpu_r.ACC_X;
        ayt+=mpu_r.ACC_Y;
        azt+=mpu_r.ACC_Z;
        gxt+=mpu_r.GYR_X;
        gyt+=mpu_r.GYR_Y;
        gzt+=mpu_r.GYR_Z;
    }
    axo = axt/100;
    ayo = ayt/100;
    azo = azt/100 - 16384;
    gxo = gxt/100;
    gyo = gyt/100;
    gzo = gzt/100;
}
extern OS_EVENT* MutexSemp;
extern INT8U err;
void GY86_getdata(void){
    MPU_Get();
		//单位m/s2
    mpu.ACC_X = (mpu_r.ACC_X-axo)/ACCEL_2G;
    mpu.ACC_Y = (mpu_r.ACC_Y-ayo)/ACCEL_2G;
    mpu.ACC_Z = (mpu_r.ACC_Z-azo)/ACCEL_2G;
		//单位°/s
    mpu.GYR_X = (mpu_r.GYR_X-gxo)/GYRO_250DPS;
    mpu.GYR_Y = (mpu_r.GYR_Y-gyo)/GYRO_250DPS;
    mpu.GYR_Z = (mpu_r.GYR_Z-gzo)/GYRO_250DPS;
    HMC_Get();//地磁数据未处理
   
}

void angle_show(void){
		sprintf(show10,"p:%2.1f",angle.pitch);
		sprintf(show11,"r:%2.1f",angle.roll);
		sprintf(show12,"y:%2.1f",angle.yaw);
		OLED_ShowStr(60, 3, show10);
		OLED_ShowStr(60, 4, show11);
		OLED_ShowStr(60, 5, show12);
}

