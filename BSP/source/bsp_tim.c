   #include "bsp_tim.h"
	 #include "bsp_oled.h"
	 

/*
	extern ֻ����,h �ļ��������ģ����ʵĶ���ͬһ������
*/
TIM_OCInitTypeDef CH1,CH2,CH3,CH4; //���Ƕ���
/*
stm32 ��ʱ����ʱ����ͨ������ ������㣬Ҳ����˵������������ж�  �����������ڼ������֣���ÿ��ʱ�����ڶ���1 ����ARR�����������ڶ���
���м������Ƕ���ʱ�������װ��
���㹫ʽ��
            �൱�ڷ�Ƶ+1 �ٳ��� ARR���е����ּ�1 ����ʱ�����ڣ���Ҳ���൱���ǳ���ʱ��Ƶ�ʣ�
TimeOut = ((Prescaler+1)*(Period+1)) /TimeClockFren;
����TimeOut:��ʱ�����ʱ��(��λΪus) ,���ٴ���(����һ��)TIM�ж�

Prescaler:��ƵTIMʱ�ӵ�Ԥ��Ƶ��ֵ
Period:��������ֵ��TIM�������������ֵ�������¼�����
TimeClockFren:��ʱ��������ʱ��Ƶ��(��λMHZ)��Ҳ���ǵ�ǰʹ�õ�TIM���õ�CLOCK��ʱ��Ƶ�ʡ�

��˶����ҵ����벶�������ʱ������ �� PPM��ʱ������һ������ÿ�ζ��ǲ���һ��PPM�� Ȼ��PPM��ֵ ����һ�����鵱��

����ȽϾ���ͨ����ʱ�����ⲿ���Ŷ�����������ź�,PWM ģʽ ��ʵҲ��������Ƚ��е�һ��ģʽ
����TIM2Ӧ�þ�������Ƚ�

�������벶�񻹿���ʹ�� PWM����ģʽ��Ҳ�൱�������벶���һ������
*/


/*
	ͨ��TIM1��CH4�������벶�����ö������ź������ػ����½��ؽ������벶��
	�������������ڱ��ͱ�խ���������Ƕ��������ؽ��в���
	���ϣ�������źŵ�ÿһ�����أ��򲻷�Ƶ
	��Ȼ�������������жϣ������Ҳ�û��ʹ����
*/

void TIM1_CONFIG(void){
	GPIO_InitTypeDef GPIO;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	//����GPIO
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);	
	GPIO.GPIO_Mode=GPIO_Mode_AF;
	GPIO.GPIO_OType=GPIO_OType_PP;
	GPIO.GPIO_Pin=TIM1_CH4_Pin;
	GPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(TIM1_GPIO,&GPIO);
	//TIM1 ʱ�� �ĳ�ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update );
	TIM_TimeBaseInitTypeDef TIM; 
	TIM.TIM_ClockDivision=TIM_CKD_DIV1 ;
	TIM.TIM_CounterMode=TIM_CounterMode_Up;
	TIM.TIM_Prescaler=420-1;
	TIM.TIM_RepetitionCounter=0;
	TIM.TIM_Period=(0xffff-1);
	TIM_TimeBaseInit(TIM1,&TIM);
	//TIM1 ���벶���ʼ��
	TIM_ICInitTypeDef TIM_IC;
	TIM_IC.TIM_Channel=TIM_Channel_4;
	TIM_IC.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_IC.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_IC.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_IC.TIM_ICFilter=0x00;
	TIM_ICInit(TIM1,&TIM_IC);
	//����ʱ���ж�
	NVIC_InitTypeDef NVICA;
	NVICA.NVIC_IRQChannel=TIM1_CC_IRQn;
	NVICA.NVIC_IRQChannelPreemptionPriority=1;
	NVICA.NVIC_IRQChannelSubPriority=2;
	NVICA.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVICA);
	TIM_ClearFlag(TIM1,TIM_IT_CC4|TIM_IT_Update);
	TIM_ITConfig(TIM1,TIM_IT_CC4,ENABLE);//|TIM_IT_Update,ENABLE);//���벶���жϣ���Update��� �ж�
	TIM_Cmd(TIM1,ENABLE);
}



/*
	PWM�źŵ��ź�Ƶ�� F=TIM_CLK/(ARR+1)*(PSC+1);
	ʹ��Ԥװ�ؼĴ�������һ�����û��޷������ģ����ڲ��������������õļĴ�����Ϊshadow register��
	�����޸ĵĶ�ʱ�����ڣ�Ԥ��Ƶϵ����ͨ���ıȽ�ֵ�ȶ����޸ĵı������Ԥװ�ؼĴ�����Ҫ������޸�
	�����ã��ͻ�Ҫ��Ԥװ�ؼĴ�����ֵ����Ӱ�ӼĴ����ſ�ʹ��
	4��ͨ���Ĳ���/�ȽϼĴ���Ҳ��ͬ���ĵ�����CCRx��Ԥװ�ؼĴ�������Ӱ�ӼĴ����������λ����
	OC1PE����ARRԤװ�ؼĴ����͵�Ӱ�ӼĴ����������ַ�ʽ��һ�������̸��£�һ���ǵȴ����¼�֮����£�
*/
void TIM3_CONFIG(void){
	GPIO_InitTypeDef GPIO;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//����GPIO
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);
	GPIO.GPIO_Mode=GPIO_Mode_AF;
	GPIO.GPIO_OType=GPIO_OType_PP;
	GPIO.GPIO_Pin=TIM3_CH1_Pin|TIM3_CH2_Pin;
	GPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO);
	GPIO.GPIO_Pin=TIM3_CH3_Pin|TIM3_CH4_Pin;
	GPIO_Init(GPIOB,&GPIO);
	//TIM3ʱ���ĳ�ʼ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
	TIM_TimeBaseInitTypeDef TIM;
	TIM.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM.TIM_CounterMode=TIM_CounterMode_Up;
	//Ƶ����50hz
	TIM.TIM_Period=20000-1;
	TIM.TIM_Prescaler=84-1;
	TIM.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM);
	CH1.TIM_OCMode=TIM_OCMode_PWM1;
	CH1.TIM_OutputState=TIM_OutputState_Enable;
	CH1.TIM_OCPolarity=TIM_OCPolarity_High;
	CH1.TIM_Pulse=2000;
	TIM_OC1Init(TIM3,&CH1);
	//����֮��ſ��������е�ʱ��ɱ�̵��޸� compare��ֵ
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	CH2.TIM_OCMode=TIM_OCMode_PWM1;
	CH2.TIM_OutputState=TIM_OutputState_Enable;
	CH2.TIM_OCPolarity=TIM_OCPolarity_High;
	CH2.TIM_Pulse=2000;
	TIM_OC2Init(TIM3,&CH2);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	CH3.TIM_OCMode=TIM_OCMode_PWM1;
	CH3.TIM_OutputState=TIM_OutputState_Enable;
	CH3.TIM_OCPolarity=TIM_OCPolarity_High;
	CH3.TIM_Pulse=2000;
	TIM_OC3Init(TIM3,&CH3);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	CH4.TIM_OCMode=TIM_OCMode_PWM1;
	CH4.TIM_OutputState=TIM_OutputState_Enable;
	CH4.TIM_OCPolarity=TIM_OCPolarity_High;
	CH4.TIM_Pulse=2000;
	TIM_OC4Init(TIM3,&CH4);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

//������������

void TIM4_CONFIG(void){
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	TIM_TimeBaseInitTypeDef TIM;
	TIM.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM.TIM_CounterMode=TIM_CounterMode_Up;
	//Ƶ����1khz
	TIM.TIM_Period=1000-1;
	TIM.TIM_Prescaler=84-1;
	TIM.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4,&TIM);
	//TIM_ARRPreloadConfig(TIM4,ENABLE);
}

void TIM2_CONFIG(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	
	TIM_TimeBaseInitTypeDef TIM;
	TIM.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM.TIM_CounterMode=TIM_CounterMode_Up;
	//Ƶ����1khz
	TIM.TIM_Period=0xffff-1;
	TIM.TIM_Prescaler=84-1;
	TIM.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM);
	TIM_Cmd(TIM2,ENABLE);
}

void TIM5_CONFIG(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	
	TIM_TimeBaseInitTypeDef TIM;
	TIM.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM.TIM_CounterMode=TIM_CounterMode_Up;
	//Ƶ����1Mhz
	TIM.TIM_Period=0xffff-1;
	TIM.TIM_Prescaler=84-1;
	TIM.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM5,&TIM);
	TIM_Cmd(TIM5,ENABLE);
}

float PID_GETTIME(void){
	static unsigned int num=0;
	if(num==1){
		TIM5->CNT=0;
		num=0;//�´ο�ʼ����
	}else {
		float time=0.0f;
		int now=0;
		now=TIM5->CNT;
		time=(now)/2000000.0;
		num=1;
		return time;
	}
	return 0;
}

float GY86_GETTIME(void){
	static unsigned int num=0;
	if(num==1){//��ȡstart ʱ��
		TIM2->CNT=0;
		num=0;
	}else{
		float time=0.0f;
		int now=0;
		now=TIM2->CNT;
		//����2Mhz��ʾ ��
		time=(now)/2000000.0;
		num=1;
		return time;
	}
	return 0;//ִ�е����ʾ��start
}
	
void TIMDelay(uint32_t Times){
	TIM_Cmd(TIM4,ENABLE);
	while(Times--){
		while(!(TIM4->SR& TIM_SR_UIF)){}
		TIM4->SR &=~TIM_SR_UIF;//��UIF ��0 ������жϱ�־
	}
	TIM_Cmd(TIM4,DISABLE);
}











