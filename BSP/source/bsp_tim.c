   #include "bsp_tim.h"
	 #include "bsp_oled.h"
	 

/*
	extern 只是在,h 文件中声明的，访问的都是同一个东西
*/
TIM_OCInitTypeDef CH1,CH2,CH3,CH4; //这是定义
/*
stm32 定时器定时计算通过计数 溢出计算，也就是说计数溢出触发中断  计数器是用于计算数字，即每个时钟周期都加1 但是ARR是用于设置在多少
进行计数器是多大的时候进行重装载
计算公式：
            相当于分频+1 再乘以 ARR当中的数字加1 乘以时钟周期，（也就相当于是除以时钟频率）
TimeOut = ((Prescaler+1)*(Period+1)) /TimeClockFren;
定义TimeOut:定时器溢出时间(单位为us) ,多少触发(进入一次)TIM中断

Prescaler:分频TIM时钟的预分频器值
Period:计数重载值，TIM计数当超过这个值，则重新计数。
TimeClockFren:定时器的输入时钟频率(单位MHZ)，也就是当前使用的TIM所用的CLOCK的时钟频率。

因此对于我的输入捕获，我需把时钟周期 与 PPM的时钟周期一样，即每次都是捕获一个PPM， 然后将PPM的值 捕获到一个数组当中

输出比较就是通过定时器的外部引脚对外输出控制信号,PWM 模式 其实也算是输出比较中的一种模式
所以TIM2应该就是输出比较

对于输入捕获还可以使用 PWM输入模式，也相当于是输入捕获的一个特例
*/


/*
	通过TIM1的CH4进行输入捕获，设置对输入信号上升沿还是下降沿进行输入捕获
	由于是上升沿在变宽和变窄，所以我是对于上升沿进行捕获
	如果希望捕获信号的每一个边沿，则不分频
	虽然设置了上升沿中断，但是我并没有使用他
*/

void TIM1_CONFIG(void){
	GPIO_InitTypeDef GPIO;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	//配置GPIO
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);	
	GPIO.GPIO_Mode=GPIO_Mode_AF;
	GPIO.GPIO_OType=GPIO_OType_PP;
	GPIO.GPIO_Pin=TIM1_CH4_Pin;
	GPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(TIM1_GPIO,&GPIO);
	//TIM1 时基 的初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update );
	TIM_TimeBaseInitTypeDef TIM; 
	TIM.TIM_ClockDivision=TIM_CKD_DIV1 ;
	TIM.TIM_CounterMode=TIM_CounterMode_Up;
	TIM.TIM_Prescaler=420-1;
	TIM.TIM_RepetitionCounter=0;
	TIM.TIM_Period=(0xffff-1);
	TIM_TimeBaseInit(TIM1,&TIM);
	//TIM1 输入捕获初始化
	TIM_ICInitTypeDef TIM_IC;
	TIM_IC.TIM_Channel=TIM_Channel_4;
	TIM_IC.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_IC.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_IC.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_IC.TIM_ICFilter=0x00;
	TIM_ICInit(TIM1,&TIM_IC);
	//设置时钟中断
	NVIC_InitTypeDef NVICA;
	NVICA.NVIC_IRQChannel=TIM1_CC_IRQn;
	NVICA.NVIC_IRQChannelPreemptionPriority=1;
	NVICA.NVIC_IRQChannelSubPriority=2;
	NVICA.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVICA);
	TIM_ClearFlag(TIM1,TIM_IT_CC4|TIM_IT_Update);
	TIM_ITConfig(TIM1,TIM_IT_CC4,ENABLE);//|TIM_IT_Update,ENABLE);//输入捕获中断，和Update溢出 中断
	TIM_Cmd(TIM1,ENABLE);
}



/*
	PWM信号的信号频率 F=TIM_CLK/(ARR+1)*(PSC+1);
	使能预装载寄存器，另一个是用户无法看见的，但在操作中真正起作用的寄存器称为shadow register，
	我们修改的定时器周期，预分频系数，通道的比较值等都是修改的表面这个预装载寄存器，要让这个修改
	起作用，就还要把预装载寄存器的值赋给影子寄存器才可使用
	4个通道的捕获/比较寄存器也是同样的道理，从CCRx的预装载寄存器传到影子寄存器由下面的位控制
	OC1PE，从ARR预装载寄存器送到影子寄存器，有两种方式，一种是立刻更新，一种是等触发事件之后更新，
*/
void TIM3_CONFIG(void){
	GPIO_InitTypeDef GPIO;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//配置GPIO
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
	//TIM3时基的初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
	TIM_TimeBaseInitTypeDef TIM;
	TIM.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM.TIM_CounterMode=TIM_CounterMode_Up;
	//频率是50hz
	TIM.TIM_Period=20000-1;
	TIM.TIM_Prescaler=84-1;
	TIM.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM);
	CH1.TIM_OCMode=TIM_OCMode_PWM1;
	CH1.TIM_OutputState=TIM_OutputState_Enable;
	CH1.TIM_OCPolarity=TIM_OCPolarity_High;
	CH1.TIM_Pulse=2000;
	TIM_OC1Init(TIM3,&CH1);
	//配置之后才可以在运行的时候可编程的修改 compare的值
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

//这配置有问题

void TIM4_CONFIG(void){
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	TIM_TimeBaseInitTypeDef TIM;
	TIM.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM.TIM_CounterMode=TIM_CounterMode_Up;
	//频率是1khz
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
	//频率是1khz
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
	//频率是1Mhz
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
		num=0;//下次开始计数
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
	if(num==1){//读取start 时间
		TIM2->CNT=0;
		num=0;
	}else{
		float time=0.0f;
		int now=0;
		now=TIM2->CNT;
		//除以2Mhz表示 秒
		time=(now)/2000000.0;
		num=1;
		return time;
	}
	return 0;//执行到这表示是start
}
	
void TIMDelay(uint32_t Times){
	TIM_Cmd(TIM4,ENABLE);
	while(Times--){
		while(!(TIM4->SR& TIM_SR_UIF)){}
		TIM4->SR &=~TIM_SR_UIF;//将UIF 置0 ，清楚中断标志
	}
	TIM_Cmd(TIM4,DISABLE);
}











