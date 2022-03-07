#include "bsp_tim.h"
/*
stm32 ��ʱ����ʱ����ͨ������������㣬Ҳ����˵������������ж�
���㹫ʽ��
TimeOut = ((Prescaler+1)*(Period+1)) /TimeClockFren;
����TimeOut:��ʱ�����ʱ��(��λΪus) ,���ٴ���(����һ��)TIM�ж�
Prescaler:��ƵTIMʱ�ӵ�Ԥ��Ƶ��ֵ
Period:��������ֵ��TIM�������������ֵ�������¼�����
TimeClockFren:��ʱ��������ʱ��Ƶ��(��λMHZ)��Ҳ���ǵ�ǰʹ�õ�TIM���õ�CLOCK��ʱ��Ƶ�ʡ�

*/
extern int time;

void TIM_Delay(int ti){

		while(time!=ti);
		time=0;
}


//ʹ��TIM4 ��Ϊ��ʱ��
static void TIM_Clock_Config(){
	TIM_TimeBaseInitTypeDef tim;
	//����TIM14��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	tim.TIM_Period=Period ;
	tim.TIM_Prescaler=Prescaler;
	TIM_TimeBaseInit(TIM4,&tim);
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	//�����ж�
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
	//��GPIO ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//ͨ��1
	GPIO.GPIO_Mode=GPIO_Mode_AF;
	GPIO.GPIO_OType=GPIO_OType_PP;
	GPIO.GPIO_Pin=TIM1_CH1_Pin;
	GPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(TIM1_GPIO,&GPIO);
	//ͨ��2
	GPIO.GPIO_Pin=TIM1_CH2_Pin;
	GPIO_Init(TIM1_GPIO,&GPIO);
	//ͨ��3
	GPIO.GPIO_Pin=TIM1_CH3_Pin;
	GPIO_Init(TIM1_GPIO,&GPIO);
	//ͨ��4
	GPIO.GPIO_Pin=TIM1_CH4_Pin;
	GPIO_Init(TIM1_GPIO,&GPIO);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
	//TIM1 ʱ�� �ĳ�ʼ��
	TIM_TimeBaseInitTypeDef TIM;
	TIM.TIM_ClockDivision=1;
	//����������ģʽ������Ϊ���ϼ���
	TIM.TIM_CounterMode=TIM_CounterMode_Up;
	//����Ԥ��Ƶ 
	TIM.TIM_Prescaler=1;
	TIM.TIM_RepetitionCounter=0;
	//����CNT��������ʱ��=ʱ��/psc+1
	TIM.TIM_Period=8399;
	TIM_TimeBaseInit(TIM1,&TIM);
	
	//TIM1 ����ȽϽṹ���ʼ��
	TIM_OCInitTypeDef TIM_OCINIT;
	//����Ϊ PWMģʽ1
	TIM_OCINIT.TIM_OCMode=TIM_OCMode_PWM1;
	//���ʹ��
	TIM_OCINIT.TIM_OutputState=TIM_OutputState_Enable;
	
	//����ռ�ձ�
	TIM_OCINIT.TIM_Pulse=TIM1_PULSE;
	//���ͨ����ƽ��������  �൱�ڸߵ�ƽ��Ч
	TIM_OCINIT.TIM_OCPolarity=TIM_OCPolarity_High;
	//���ͨ�����е�ƽ��������   ���е�ʱ��Ϊ �͵�ƽ
	TIM_OCINIT.TIM_OCIdleState=TIM_OCIdleState_Set;
	//���Ƕ���ͨ��1 ��������
	TIM_OC1Init(TIM1,&TIM_OCINIT);
	//�Զ���װ��
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1,ENABLE);
		
	//�����ʹ�ܣ���ʹ��ͨ�ü�ʱ��ʱ����仰����Ҫ
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	 
}






















