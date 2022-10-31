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
	��ҪдTIM1_CH4 ���жϺ���������PPM�źŴ洢��PPM_Databuf��
	�ں�������ݽ��н�����ʱ�򣬲�һ��Ҫ��ȫ�İ��� ���õ������ݽ��д���
	�ǿ��Խ�����ͨ��һ�������Ժ����������ת��
	��ҪдTIM2��NVIC_Config����
*/
	
void TIM1_CC_IRQHandler(void){
	static int in_num=0; 
	static int status=0;
	static int time_num[9]={0};
	int num=0;
	/*
		status ����0 ��ʾ��û���ҵ���һ���ŵ���λ�ã�����ʱ����Ѱ�ҵ�һ���ŵ���λ��
	*/
	if(status==0){
		/*
			in_num==0 ��ʾ�����ǿ�ʼ�׶Σ����ǻ�ȡ�׶Σ���CNT��ֵ��0
		*/
		if(in_num==0){
				TIM1->CNT=0X00;
				in_num=1;
				TIM1->CCER|=(1<<13);//�޸�Ϊ��׽�½���
			}else{
				num=TIM_GetCapture4(TIM1);
				TIM1->CCER&=~(1<<13);//�޸�Ϊ��׽������
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
				TIM1->CCER|=(1<<13);//�޸�Ϊ��׽�½���
			}else{
				TIM1_DataBuf[in_num]=TIM_GetCapture4(TIM1);
				TIM1->CCER&=~(1<<13);//�޸�Ϊ��׽������
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
	�򵽵���200�� �ﵽ�����400 ���������ڻ���ƫ�� ����׼����202��ʱ����Ǵﵽ��;��������У���398��ʱ��������
*/

void Fly_Start(void){
	while(TIM1_DataBuf[4]<350){
	}

}


/*
	��Ҫ�޸������PWM��ֵ �˽⵽ ���Ҫ�޸�ռ�ձ� ֻ��Ҫ�޸�Compare��ֵ
	�����������ռ�ձ�  һ����10~20% ��ռ�ձ� 
	��������� �����������  ���Խ��ܵ�Compare��ֵ�� 20000/10=2000 20000/5=4000
	����ֵ����[2000,4000]
	�Ա����Ķ�������[200,400]; 
	Ϊ�˱�������İ�ȫ �Ƿ�������Ե�ӳ�亯��
*/

/*
header 
		|
		|
CH3   CH1
	\___/
	/		\
CH4   CH2

buf[0] �����ҷ���
buf[1] ��ǰ����
buf[2] �����ŷ���
buf[3] ������ת������ת����

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


