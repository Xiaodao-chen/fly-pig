#include "bsp_ppm.h"
#include "ucos_ii.h"
short TIM1_DataBuf[9]={0};	
char a[50]={'0'};
char b[50]={'0'};
char c[50]={'0'};
char d[50]={'0'};
char e[50]={'0'};
char f[50]={'0'};
char g[50]={'0'};
char h[50]={'0'};
char i[50]={'0'};
unsigned int T1=0,T2=0,T3=0,T4=0;

short CH_OUT[4]={1000};



/*
	需要写TIM1_CH4 的中断函数，接受PPM信号存储在PPM_Databuf中
	在后面对数据进行解析的时候，不一定要完全的按照 所得到的数据进行处理，
	是可以将数据通过一个非线性函数将其进行转换
	需要写TIM2的NVIC_Config函数
*/
	
void TIM1_CC_IRQHandler(void){
	static int in_num=0; 
	static int status=0;
	static int time_num[9]={0};
	int num=0;
	/*
		status 等于0 表示还没有找到第一个信道的位置，即此时是在寻找第一个信道的位置
	*/
	if(status==0){
		/*
			in_num==0 表示现在是开始阶段，还是获取阶段，将CNT的值给0
		*/
		if(in_num==0){
				TIM1->CNT=0X00;
				in_num=1;
				TIM1->CCER|=(1<<13);//修改为捕捉下降沿
			}else{
				num=TIM_GetCapture4(TIM1);
				TIM1->CCER&=~(1<<13);//修改为捕捉上升沿
				in_num=0;
		}
		if(num>1000){
			status=1;
		}
	}else{
		if(in_num<9){
			if(time_num[in_num]==0){
				TIM1->CNT=0X00;
				time_num[in_num]=1;
				TIM1->CCER|=(1<<13);//修改为捕捉下降沿
			}else{
				TIM1_DataBuf[in_num]=TIM_GetCapture4(TIM1);
				TIM1->CCER&=~(1<<13);//修改为捕捉上升沿
				time_num[in_num]=0;
				in_num++;
			}
		}else{
			in_num=0;
		}
	}
}



void Show_PPM(void){
	sprintf(a,"the 1 is %d",TIM1_DataBuf[0]);  
	sprintf(b,"the 2 is %d",TIM1_DataBuf[1]);
	sprintf(c,"the 3 is %d",TIM1_DataBuf[2]);
	sprintf(d,"the 4 is %d",TIM1_DataBuf[3]);
 	sprintf(e,"the 5 is %d",TIM1_DataBuf[4]);
	sprintf(f,"the 6 is %d",TIM1_DataBuf[5]);
	sprintf(g,"the 7 is %d",TIM1_DataBuf[6]);
	sprintf(h,"the 8 is %d",TIM1_DataBuf[7]);
	OLED_ShowStr(0,7,a,1);
	OLED_ShowStr(0,0,b,1);
	OLED_ShowStr(0,1,c,1);
	OLED_ShowStr(0,2,d,1);
	OLED_ShowStr(0,3,e,1);
	OLED_ShowStr(0,4,f,1);
	OLED_ShowStr(0,5,g,1);
	OLED_ShowStr(0,6,h,1);
}

/**
	打到底是200， 达到最高是400 ，但是由于会有偏差 所以准备在202的时候就是达到最低就允许运行，在398的时候就是最大
*/

void Fly_Start(void){
	while(TIM1_DataBuf[4]<350){
	}

}


/*
	需要修改输出的PWM的值 了解到 如果要修改占空比 只需要修改Compare的值
	这就是在配置占空比  一般是10~20% 的占空比 
	所以这就是 会产生函数，  可以接受的Compare的值是 20000/10=2000 20000/5=4000
	所以值域是[2000,4000]
	自变量的定义域是[200,400]; 
	为了保护机身的安全 是否采用线性的映射函数
*/

/*
header 
		|
		|
CH3   CH1
	\___/
	/		\
CH4   CH2

buf[0] 是左右方向
buf[1] 是前后方向
buf[2] 是油门方向
buf[3] 是左旋转和右旋转方向

*/ 






int  ChangeToPWM(int num){
	return 5*num;
}

void CH1_Change(int Compare){
	TIM_SetCompare1(TIM3,ChangeToPWM(Compare));
}


void CH2_Change(int Compare){
		TIM_SetCompare2(TIM3,ChangeToPWM(Compare));
}

void CH3_Change(int Compare){
	TIM_SetCompare3(TIM3,ChangeToPWM(Compare));
}

void CH4_Change(int Compare){
	TIM_SetCompare4(TIM3,ChangeToPWM(Compare));
}


void PPM_Init(void){
	TIM_SetCompare1(TIM3,2000);
	TIM_SetCompare2(TIM3,2000);
	TIM_SetCompare3(TIM3,2000);
	TIM_SetCompare4(TIM3,2000);
	
	TIMDelay(4000);
	TIM_SetCompare1(TIM3,1000);
	TIM_SetCompare2(TIM3,1000);
	TIM_SetCompare3(TIM3,1000);
	TIM_SetCompare4(TIM3,1000);
}


