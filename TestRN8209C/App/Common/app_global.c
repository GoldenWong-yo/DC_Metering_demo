#include "app_global.h"





void My_System_Init(void)
{
	SCB->VTOR = STM32_APP_START_ADDR;
}

void Debug_MCU_Freeeze(void)
{
	//__HAL_RCC_DBGMCU_CLK_ENABLE();
	//__HAL_DBGMCU_FREEZE_IWDG(); 	// 在调试单步时，iwdg暂停	The independent watchdog counter clock is stopped when the core is halted
	//__DBGMCU_CLK_ENABLE();
	//__HAL_FREEZE_IWDG_DBGMCU(); 	// 在调试单步时，iwdg暂停	The independent watchdog counter clock is stopped when the core is halted
}

void Reset_Tick(u32 *LastTick)
{
	*LastTick = HAL_GetTick();
}

bool Tick_Timeout(u32 *LastTick, u32 TickTimes)
{
	u32 NowTick = 0;
	
	NowTick = HAL_GetTick();
	
	if(NowTick - *LastTick >= TickTimes)
	{
		*LastTick = NowTick;
		
		return true;
	}
	
	return false;
}


