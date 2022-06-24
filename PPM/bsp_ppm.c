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
int T1=0,T2=0,T3=0,T4=0;



/*
	��ҪдTIM1_CH2 ���жϺ���������PPM�źŴ洢��PPM_Databuf��
	�ں�������ݽ��н�����ʱ�򣬲�һ��Ҫ��ȫ�İ��� ���õ������ݽ��д���
	�ǿ��Խ�����ͨ��һ�������Ժ����������ת��
	��ҪдTIM2��NVIC_Config����
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
//	sprintf(e,"the 5 is %d",TIM1_DataBuf[4]);
//	sprintf(f,"the 6 is %d",TIM1_DataBuf[5]);
//	sprintf(g,"the 7 is %d",TIM1_DataBuf[6]);
//	sprintf(h,"the 8 is %d",TIM1_DataBuf[7]);
	OLED_ShowStr(0,7,a);
	OLED_ShowStr(0,0,b);
	OLED_ShowStr(0,1,c);
	OLED_ShowStr(0,2,d);
//	OLED_ShowStr(0,3,e);
//	OLED_ShowStr(0,4,f);
//	OLED_ShowStr(0,5,g);
//	OLED_ShowStr(0,6,h);
}

/**
	�򵽵���200�� �ﵽ�����400 ���������ڻ���ƫ�� ����׼����202��ʱ����Ǵﵽ��;��������У���398��ʱ��������
*/

void Fly_Start(void){
	while(TIM1_DataBuf[0]>205||TIM1_DataBuf[1]>205||TIM1_DataBuf[2]>205||TIM1_DataBuf[3]<395){
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
CH1   CH3
	\___/
	/		\
CH2   CH4

buf[0] �����ҷ���
buf[1] ��ǰ����
buf[2] �����ŷ���
buf[3] ������ת������ת����

*/
void ChangeMotor(void){

		// ���ҷ���          ǰ����                ���Ŵ�С             ��ת����  
	T1=TIM1_DataBuf[0]-300+300-TIM1_DataBuf[1]+TIM1_DataBuf[2]+TIM1_DataBuf[3]-300;
	T2=TIM1_DataBuf[0]-300+TIM1_DataBuf[1]-300+TIM1_DataBuf[2]+300-TIM1_DataBuf[3];
	T3=300-TIM1_DataBuf[0]+300-TIM1_DataBuf[1]+TIM1_DataBuf[2]+300-TIM1_DataBuf[3];
	T4=300-TIM1_DataBuf[0]+TIM1_DataBuf[1]-300+TIM1_DataBuf[2]+TIM1_DataBuf[3]-300;
	TIM_SetCompare1(TIM3,ChangeToPWM(T1));
	TIM_SetCompare2(TIM3,ChangeToPWM(T2));
	TIM_SetCompare3(TIM3,ChangeToPWM(T3));
	TIM_SetCompare4(TIM3,ChangeToPWM(T4));
}


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
	Systick_Delay_ms(4000);
	TIM_SetCompare1(TIM3,1000);
	TIM_SetCompare2(TIM3,1000);
	TIM_SetCompare3(TIM3,1000);
	TIM_SetCompare4(TIM3,1000);
	
	
}


