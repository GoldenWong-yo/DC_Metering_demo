#include "app_led.h"
#include "app_global.h"
#include "gpio.h"





typedef enum{
	LED_OFF = 0,
	LED_ON,
}EN_LED_PIN_STATE;

typedef enum{
	LED_GLOW_ON = 0,
	LED_GLOW_OFF,
	LED_GLOW_IDLE,
	LED_GLOW_MAX
}EN_LED_GLOW_STATE;

typedef struct{
	EN_LED_SHOW ShowFunc;
	EN_LED_GLOW_STATE GlowState;
	u8 GlowCount;
	u8 IdleCount;
	u32 WaitTick;
}ST_LED;

static ST_LED Led[DEV_LED_MAX];


void Init_Led_Variables(void)
{
	memset(Led, 0, sizeof(Led));
	
	Set_LED(DEV_LED_RUN, LED_SHOW_GLOW_1_IDLE);
}

void Set_LED(EN_DEV_LED LedDev, EN_LED_SHOW ShowFunc)
{
	Led[LedDev].ShowFunc = ShowFunc;
}

static void Set_LED_Pin(EN_DEV_LED LedDev, EN_LED_PIN_STATE State)
{
	GPIO_TypeDef *GPIOx = LED_RUN_GPIO_Port;
	uint16_t GPIO_Pin = LED_RUN_Pin;
	GPIO_PinState PinState = GPIO_PIN_RESET;

	if(LedDev >= DEV_LED_MAX)
	{
		return;
	}

	switch(LedDev)
	{
		case DEV_LED_RUN:
			GPIOx = LED_RUN_GPIO_Port;
			GPIO_Pin = LED_RUN_Pin;
			break;
		case DEV_LED_WARN:
			GPIOx = LED_WARN_GPIO_Port;
			GPIO_Pin = LED_WARN_Pin;
			break;
		case DEV_LED_ERROR:
			GPIOx = LED_ERROR_GPIO_Port;
			GPIO_Pin = LED_ERROR_Pin;
			break;
		default:
			break;
	}

	if(State)
	{
		PinState = GPIO_PIN_RESET;
	}else{
		PinState = GPIO_PIN_SET;
	}

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

static void LED_Glow_Func(EN_DEV_LED LedDev, u8 GlowTimes)
{
	switch(Led[LedDev].GlowState)
	{
		case LED_GLOW_ON:
			if(Tick_Timeout(&Led[LedDev].WaitTick, TIME_200MS))
			{
				Set_LED_Pin(LedDev, LED_ON);
				Led[LedDev].GlowState = LED_GLOW_OFF;
			}
			break;
		case LED_GLOW_OFF:
			if(Tick_Timeout(&Led[LedDev].WaitTick, TIME_200MS))
			{
				Set_LED_Pin(LedDev, LED_OFF);

				if(GlowTimes == 0)
				{
					Led[LedDev].GlowState = LED_GLOW_ON;
				}else{
					if(++Led[LedDev].GlowCount >= GlowTimes)
					{
						Led[LedDev].GlowCount = 0;
						Led[LedDev].GlowState = LED_GLOW_IDLE;
					}else{
						Led[LedDev].GlowState = LED_GLOW_ON;
					}
				}
			}
			break;
		case LED_GLOW_IDLE:
			if(Tick_Timeout(&Led[LedDev].WaitTick, TIME_1S))
			{
				Led[LedDev].GlowState = LED_GLOW_ON;
			}
			break;
		default:
			Led[LedDev].GlowState = LED_GLOW_ON;
			break;
	}
}

void LED_Handler(void)
{
	EN_DEV_LED LedDev = DEV_LED_RUN;
	
	for(LedDev=DEV_LED_RUN; LedDev<DEV_LED_MAX; LedDev++)
	{
		switch(Led[LedDev].ShowFunc)
		{
			case LED_SHOW_OFF:
				Set_LED_Pin(LedDev, LED_OFF);
				Led[LedDev].ShowFunc = LED_SHOW_MAX;
				break;
			case LED_SHOW_ON:
				Set_LED_Pin(LedDev, LED_ON);
				Led[LedDev].ShowFunc = LED_SHOW_MAX;
				break;
			case LED_SHOW_GLOW:
				LED_Glow_Func(LedDev,0);
				break;
			case LED_SHOW_GLOW_1_IDLE:
				LED_Glow_Func(LedDev,1);
				break;
			case LED_SHOW_GLOW_2_IDLE:
				LED_Glow_Func(LedDev,2);
				break;
			case LED_SHOW_MAX:
				break;
		}
	}
}


