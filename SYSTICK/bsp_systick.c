#include "bsp_systick.h"

//û��ʹ���жϽ����ж�
//ע�ⲻҪ�����жϣ����Systick_Config����Ҫ���ĵ�
void Systick_Delay_ms(uint32_t ms){
	u32 i=0;
	SysTick_Config(SystemCoreClock / 1000);
	for (i=0;i<ms;i++){
		while(!((SysTick->CTRL) & SysTick_CTRL_COUNTFLAG_Msk));
	}
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;// ���൱�ڹرյδ�ʱ��
}













