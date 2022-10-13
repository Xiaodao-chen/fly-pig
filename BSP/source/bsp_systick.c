#include "bsp_systick.h"

//没有使用中断进行判断
//注意不要开启中断，因此Systick_Config是需要更改的
//void Systick_Delay_ms(uint32_t ms){
//	u32 i=0;
//	SysTick_Config(SystemCoreClock / 1000);
//	for (i=0;i<ms;i++){
//		while(!((SysTick->CTRL) & SysTick_CTRL_COUNTFLAG_Msk));
//	}
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;// 这相当于关闭滴答时钟
//}





//延时nus
//nus为要延时的us数.	
//nus:0~190887435(最大值即2^32/fac_us@fac_us=22.5)	 
//void Systick_Delay_ms(uint32_t nus)
//{		
//	u32 ticks;
//	u32 told,tnow,tcnt=0;
//	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
//	ticks=nus*84; 						//需要的节拍数 
//	told=SysTick->VAL;        				//刚进入时的计数器值
//	while(1)
//	{
//		tnow=SysTick->VAL;	
//		if(tnow!=told)
//		{	    
//			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
//			else tcnt+=reload-tnow+told;	    
//			told=tnow;
//			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
//		}  
//	};
//}








