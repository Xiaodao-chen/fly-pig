#include "bsp_ppm.h"
int TIM1_DataBuf[9]={0};	
char a[50]={'0'};
char b[50]={'0'};
char c[50]={'0'};
char d[50]={'0'};
char e[50]={'0'};
char f[50]={'0'};
char g[50]={'0'};
char h[50]={'0'};
char i[50]={'0'};




/*
	需要写TIM1_CH2 的中断函数，接受PPM信号存储在PPM_Databuf中
	在后面对数据进行解析的时候，不一定要完全的按照 所得到的数据进行处理，
	是可以将数据通过一个非线性函数将其进行转换
	需要写TIM2的NVIC_Config函数
*/
	
void TIM1_CC_IRQHandler(void){
	static int in_num=0; 
	static int status=0;
	static int time_num[9]={0};
	int num=0;
	if(status==0){
		if(in_num==0){
				TIM1->CNT=0X00;
				in_num=1;
			}else{
				num=TIM_GetCapture2(TIM1);
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
			}else{
				TIM1_DataBuf[in_num]=TIM_GetCapture2(TIM1);
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
	OLED_ShowStr(0,0,a);
	OLED_ShowStr(0,1,b);
	OLED_ShowStr(0,2,c);
	OLED_ShowStr(0,3,d);
	OLED_ShowStr(0,4,e);
	OLED_ShowStr(0,5,f);
	OLED_ShowStr(0,6,g);
	OLED_ShowStr(0,7,h);//
}

/**
	打到底是200， 达到最高是400 ，但是由于会有偏差 所以准备在202的时候就是达到最低就允许运行，在398的时候就是最大
*/

void Fly_Start(void){
	while(TIM1_DataBuf[0]>205||TIM1_DataBuf[1]>205||TIM1_DataBuf[2]>205||TIM1_DataBuf[3]<395){
	}
	CH1_Change(220);
	CH2_Change(220);
	CH3_Change(220);
	CH4_Change(220);
}


/*
	需要修改输出的PWM的值 了解到 如果要修改占空比 只需要修改Compare的值
	这就是在配置占空比  一般是10~20% 的占空比 
	所以这就是 会产生函数，  可以接受的Compare的值是 20000/10=2000 20000/5=4000
	所以值域是[2000,4000]
	自变量的定义域是[200,400]; 
	为了保护机身的安全 是否采用线性的映射函数
*/

int  ChangeToPWM(int num){
	return 10*num;
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



