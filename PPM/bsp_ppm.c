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

TIM_OCInitTypeDef C1,C2,C3,C4;


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
		if(num>100){
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
	sprintf(i,"the 9 is %d",TIM1_DataBuf[8]);
	OLED_ShowStr(0,0,a);
	OLED_ShowStr(0,1,b);
	OLED_ShowStr(0,2,c);
	OLED_ShowStr(0,3,d);
	OLED_ShowStr(0,4,e);
	OLED_ShowStr(0,5,f);
	OLED_ShowStr(0,6,g);
	OLED_ShowStr(0,7,h);
}




/*
	��Ҫ�޸������PWM��ֵ ����
*/

void CH1_Change(){
	
	
}


void CH2_Change(){
	
	
}

void CH3_Change(){
	
	
}

void CH4_Change(){
	
	
}



