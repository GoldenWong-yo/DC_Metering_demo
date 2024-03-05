#include "app_printf.h"
#include "app_global.h"
//#include "usart.h"





#define PRINTF_UART USART1

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif


PUTCHAR_PROTOTYPE
{
	//具体哪个串口可以更改USART1为其它串口
	while((PRINTF_UART->SR & 0X40) == 0);	//等待发送完成标志
	PRINTF_UART->DR = (u8)ch;				//发送

	return ch;
}



