#include "bsp_systick.h"

//û��ʹ���жϽ����ж�
//ע�ⲻҪ�����жϣ����Systick_Config����Ҫ���ĵ�
//void Systick_Delay_ms(uint32_t ms){
//	u32 i=0;
//	SysTick_Config(SystemCoreClock / 1000);
//	for (i=0;i<ms;i++){
//		while(!((SysTick->CTRL) & SysTick_CTRL_COUNTFLAG_Msk));
//	}
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;// ���൱�ڹرյδ�ʱ��
//}





//��ʱnus
//nusΪҪ��ʱ��us��.	
//nus:0~190887435(���ֵ��2^32/fac_us@fac_us=22.5)	 
//void Systick_Delay_ms(uint32_t nus)
//{		
//	u32 ticks;
//	u32 told,tnow,tcnt=0;
//	u32 reload=SysTick->LOAD;				//LOAD��ֵ	    	 
//	ticks=nus*84; 						//��Ҫ�Ľ����� 
//	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
//	while(1)
//	{
//		tnow=SysTick->VAL;	
//		if(tnow!=told)
//		{	    
//			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
//			else tcnt+=reload-tnow+told;	    
//			told=tnow;
//			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
//		}  
//	};
//}








