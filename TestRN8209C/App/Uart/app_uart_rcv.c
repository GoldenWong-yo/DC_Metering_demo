#include "app_uart_rcv.h"
#include "app_protocol_ydt1363.h"
#include "app_printf.h"
#include "usart.h"
#include "app_tool.h"





ST_UART_DATA UartRcv;


void Send_Data(EN_DEV_UART DevUart, ST_DATA TxData)
{
	if(DevUart == DEV_UART_1)
	{
		memcpy(Uart1.Tx.pBuf, TxData.Temp1.pBuf, TxData.Temp1.Len);
		Uart1.Tx.Len = TxData.Temp1.Len;
		
		HAL_UART_Transmit_DMA(&huart1, Uart1.Tx.pBuf, Uart1.Tx.Len);
	}
	else if(DevUart == DEV_UART_2)
	{
		memcpy(Uart2.Tx.pBuf, TxData.Temp1.pBuf, TxData.Temp1.Len);
		Uart2.Tx.Len = TxData.Temp1.Len;
		
		HAL_UART_Transmit_DMA(&huart2, Uart2.Tx.pBuf, Uart2.Tx.Len);
	}
}

typedef enum{
	NullData_Received = 0,
	Received_NewData
}EN_ReceiveData_Status;

static EN_ReceiveData_Status If_Receive_Data(void)
{
	EN_ReceiveData_Status Status = NullData_Received;
	
	if(Uart1.Rx.Len > 0)
	{
		memcpy(UartRcv.Data.Temp1.pBuf, Uart1.Rx.pBuf, Uart1.Rx.Len);
		UartRcv.Data.Temp1.Len = Uart1.Rx.Len;
		UartRcv.DevUart = DEV_UART_1;
		
		Uart1.Rx.Len = 0;
		
		Status = Received_NewData;
	}
	else if(Uart2.Rx.Len > 0)
	{
		memcpy(UartRcv.Data.Temp1.pBuf, Uart2.Rx.pBuf, Uart2.Rx.Len);
		UartRcv.Data.Temp1.Len = Uart2.Rx.Len;
		UartRcv.DevUart = DEV_UART_2;
		
		Uart2.Rx.Len = 0;
		
		Status = Received_NewData;
	}
	
	return Status;
}

static void Check_Package(void)
{
	if(If_Protocol_YDT())
	{
		//铁塔协议
		Check_Protocol_YDT();
	}
}

#define UART_BUF_SIZE UART1_BUF_SIZE

void Uart_Handler(void)
{
	EN_ReceiveData_Status ReceiveDataStatus = NullData_Received;
	u8 TempBuf1[UART_BUF_SIZE] = {0};
	u8 TempBuf2[UART_BUF_SIZE] = {0};

	memset(TempBuf1, 0, sizeof(TempBuf1));
	memset(TempBuf2, 0, sizeof(TempBuf1));

	UartRcv.Data.Temp1.pBuf = TempBuf1;
	UartRcv.Data.Temp2.pBuf = TempBuf2;

	ReceiveDataStatus = If_Receive_Data();
	if(ReceiveDataStatus)
	{
		//My_Printf("Rcv Data: %s\r\n", UartRcv.Data.Temp1.pBuf);

		Check_Package();
	}
}

