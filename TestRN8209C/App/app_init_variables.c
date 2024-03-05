#include "app_init_variables.h"
#include "app_test.h"
#include "usart.h"
#include "app_storage.h"
#include "app_led.h"





void Init_Variables(void)
{
	Init_Uart_Variables();
	Init_Test_Variables();
	Init_Storage_Variables();
	Init_Led_Variables();
}

