#include "bsp_tim.h"
/*
stm32 定时器定时计算通过计数溢出计算，也就是说计数溢出触发中断
计算公式：
TimeOut = ((Prescaler+1)*(Period+1)) /TimeClockFren;
定义TimeOut:定时器溢出时间(单位为us) ,多少触发(进入一次)TIM中断
Prescaler:分频TIM时钟的预分频器值
Period:计数重载值，TIM计数当超过这个值，则重新计数。
TimeClockFren:定时器的输入时钟频率(单位MHZ)，也就是当前使用的TIM所用的CLOCK的时钟频率。

*/
extern int time;

void TIM_Delay(int ti){

		while(time!=ti);
		time=0;
}


//使用TIM4 作为计时器
static void TIM_Clock_Config(){
	TIM_TimeBaseInitTypeDef tim;
	//开启TIM14的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	tim.TIM_Period=Period ;
	tim.TIM_Prescaler=Prescaler;
	TIM_TimeBaseInit(TIM4,&tim);
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	//开启中断
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM4,ENABLE);
	
}

static void TIM_COUNT_CONFIG(){
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel=TIM4_IRQn ;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=0;
	nvic.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&nvic);
}


void TIM4_IRQHandler(){
		if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET){
			time++;
			TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
		}  
}

void TIM4_Init(void){
	TIM_Clock_Config();
	TIM_COUNT_CONFIG();
}


 void TIM1_COFIG(void){
	GPIO_InitTypeDef GPIO;
	//打开GPIO 的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//通道1
	GPIO.GPIO_Mode=GPIO_Mode_AF;
	GPIO.GPIO_OType=GPIO_OType_PP;
	GPIO.GPIO_Pin=TIM1_CH1_Pin;
	GPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(TIM1_GPIO,&GPIO);
	//通道2
	GPIO.GPIO_Pin=TIM1_CH2_Pin;
	GPIO_Init(TIM1_GPIO,&GPIO);
	//通道3
	GPIO.GPIO_Pin=TIM1_CH3_Pin;
	GPIO_Init(TIM1_GPIO,&GPIO);
	//通道4
	GPIO.GPIO_Pin=TIM1_CH4_Pin;
	GPIO_Init(TIM1_GPIO,&GPIO);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
	//TIM1 时基 的初始化
	TIM_TimeBaseInitTypeDef TIM;
	TIM.TIM_ClockDivision=1;
	//计数器计数模式，设置为向上计数
	TIM.TIM_CounterMode=TIM_CounterMode_Up;
	//设置预分频 
	TIM.TIM_Prescaler=1;
	TIM.TIM_RepetitionCounter=0;
	//驱动CNT计数器的时钟=时钟/psc+1
	TIM.TIM_Period=8399;
	TIM_TimeBaseInit(TIM1,&TIM);
	
	//TIM1 输出比较结构体初始化
	TIM_OCInitTypeDef TIM_OCINIT;
	//配置为 PWM模式1
	TIM_OCINIT.TIM_OCMode=TIM_OCMode_PWM1;
	//输出使能
	TIM_OCINIT.TIM_OutputState=TIM_OutputState_Enable;
	
	//配置占空比
	TIM_OCINIT.TIM_Pulse=TIM1_PULSE;
	//输出通道电平极性配置  相当于高电平有效
	TIM_OCINIT.TIM_OCPolarity=TIM_OCPolarity_High;
	//输出通道空闲电平极性配置   空闲的时候为 低电平
	TIM_OCINIT.TIM_OCIdleState=TIM_OCIdleState_Set;
	//这是对于通道1 进行配置
	TIM_OC1Init(TIM1,&TIM_OCINIT);
	//自动重装载
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1,ENABLE);
		
	//主输出使能，当使用通用计时器时，这句话不需要
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	 
}






















