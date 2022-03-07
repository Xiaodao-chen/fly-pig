#include "bsp_i2c.h"


void I2C_CONFIG(void){
	I2C_InitTypeDef I2C;
	GPIO_InitTypeDef GPIO;
	//开启i2c时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	//开启I2C GPIO 时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//配置 gpio 的复用
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C1);
	GPIO.GPIO_Mode=GPIO_Mode_AF;
	GPIO.GPIO_OType=GPIO_OType_OD;
	GPIO.GPIO_Pin=I2C1_SCL_Pin;
	GPIO.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO.GPIO_Speed=GPIO_Speed_50MHz;
	//初始化GPIO
	GPIO_Init(GPIOB,&GPIO);
	GPIO.GPIO_Pin=I2C1_SDA_Pin;
	GPIO_Init(GPIOB,&GPIO);
	
	//配置i2c 外设
	I2C.I2C_Ack=I2C_Ack_Enable;
	I2C.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	I2C.I2C_ClockSpeed=I2C_SPEED;
	I2C.I2C_DutyCycle=I2C_DutyCycle_2;
	I2C.I2C_Mode=I2C_Mode_I2C;
	I2C.I2C_OwnAddress1= I2C_OWN_ADDR;
	I2C_Init(I2C1,&I2C);
	
	//使能函数
	I2C_Cmd(I2C1,ENABLE);
}

//I2C Address Detective
void I2C_Addr_Detect(void){
    uint8_t Addr = 0;
    uint32_t cnt = 0;
    //Wait for Bus Idle
    while(I2C1->SR2 & I2C_SR2_BUSY);
    //Start Transmit
    while(Addr < 255){
        I2C1->CR1 |= I2C_CR1_START;
        while(!(I2C1->SR1 & I2C_SR1_SB));
        I2C1->DR = Addr & ~1;
        cnt = 84000;
        while(!(I2C1->SR1 & I2C_SR1_ADDR) && cnt){
            cnt--;
        }
        if(cnt == 0){
            Addr++;
            I2C1->CR1 |= I2C_CR1_STOP;
        }
        else{
            I2C1->SR2;
            printf("%x\r\n", Addr);
            I2C1->CR1 |= I2C_CR1_STOP;
            Addr++;
        }
    }
    printf("END\r\n\r\n");
}





