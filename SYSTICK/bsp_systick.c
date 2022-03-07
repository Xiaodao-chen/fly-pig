#include "bsp_systick.h"

//没有使用中断进行判断
//注意不要开启中断，因此Systick_Config是需要更改的
void Systick_Delay_ms(uint32_t ms){
	u32 i=0;
	SysTick_Config(SystemCoreClock / 1000);
	for (i=0;i<ms;i++){
		while(!((SysTick->CTRL) & SysTick_CTRL_COUNTFLAG_Msk));
	}
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;// 这相当于关闭滴答时钟
}













