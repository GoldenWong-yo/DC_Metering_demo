#ifndef _APP_UART_RCV_H_
#define _APP_UART_RCV_H_

#include "app_global.h"





typedef enum{
	DEV_UART_1 = 0,
	DEV_UART_2,
//	DEV_UART_3,
//	DEV_UART_4,
}EN_DEV_UART;

typedef struct{
	ST_BUF Temp1;			//在将数据传递给下一级函数时，默认将数据放到Temp1
	ST_BUF Temp2;
}ST_DATA;

typedef struct{
	EN_DEV_UART DevUart;
	ST_DATA Data;
}ST_UART_DATA;


extern ST_UART_DATA UartRcv;


void Send_Data(EN_DEV_UART DevUart, ST_DATA TxData);
void Uart_Handler(void);

#endif

