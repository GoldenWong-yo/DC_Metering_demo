#ifndef _APP_LED_H_
#define _APP_LED_H_





typedef enum{
	LED_SHOW_OFF = 0,
	LED_SHOW_ON,
	LED_SHOW_GLOW,
	LED_SHOW_GLOW_1_IDLE,
	LED_SHOW_GLOW_2_IDLE,
	LED_SHOW_MAX
}EN_LED_SHOW;

typedef enum{
	DEV_LED_RUN = 0,
	DEV_LED_WARN,
	DEV_LED_ERROR,
	DEV_LED_MAX
}EN_DEV_LED;


void Init_Led_Variables(void);
void Set_LED(EN_DEV_LED LedDev,EN_LED_SHOW ShowFunc);
void LED_Handler(void);


#endif

