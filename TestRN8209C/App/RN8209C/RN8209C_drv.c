#include "RN8209C_drv.h"
#include "RN8209C_reg.h"
#include "usart.h"
#include "app_tool.h"





#define RN8209C_BUF_SIZE UART3_BUF_SIZE


//****************************************************
//RN8209C发送数据
//****************************************************
static void RN8209C_Send(u8 Data[], u8 Len)
{
	HAL_GPIO_WritePin(GPIOA, RN8209C_CH_SW_B_Pin|RN8209C_CH_SW_A_Pin, GPIO_PIN_RESET);

	huart3.Init.BaudRate = 4800;
	huart3.Init.Parity = UART_PARITY_EVEN;
	if (HAL_UART_Init(&huart3) != HAL_OK)
	{
		Error_Handler();
	}

	memcpy(Uart3.Tx.pBuf, Data, Len);
	Uart3.Tx.Len = Len;

	HAL_UART_Transmit_DMA(&huart3, Uart3.Tx.pBuf, Uart3.Tx.Len);
}

static EN_Global_Status RN8209C_Check_Reg_Pack(EN_RN8209C_REG_ADDR RegAddr, u8 Data[], u8 Len)
{
	EN_Global_Status Status = Status_Error;
	u8 tmpData[RN8209C_BUF_SIZE];
	u8 i = 0;
	u8 TempCSM = 0;

	tmpData[0] = RegAddr;

	for(i=0; i<(Len-1); i++)
	{
		tmpData[i+1] = Data[i];
	}

	TempCSM = (~(Check_Sum(tmpData, Len)));

	if(Data[Len-1] == TempCSM)
	{
		Status = Status_Success;
	}

	return Status;
}

static EN_Global_Status RN8209C_Wait_Ack(EN_RN8209C_REG_ADDR RegAddr)
{
	EN_Global_Status Status = Status_Success;
	u32 WaitTick = 0;
	u8 tmpData[RN8209C_BUF_SIZE];
	u8 Len = 0;

	Reset_Tick(&WaitTick);

	while(1)
	{
		if(Uart3.Rx.Len > 0)
		{
			memcpy(tmpData, Uart3.Rx.pBuf, Uart3.Rx.Len);
			Len = Uart3.Rx.Len;

			Uart3.Rx.Len = 0;

			Status = RN8209C_Check_Reg_Pack(RegAddr, tmpData, Len);
			if(Status == Status_Success)
			{
				Status = RN8209C_Parse_Cmd(RegAddr, tmpData, (Len-1));
			}

			break;
		}
		else if(Tick_Timeout(&WaitTick, TIME_50MS))
		{
			Status = Status_Timeout;
			break;
		}
	}

	return Status;
}





//****************************************************
//RN8209C写寄存器
//****************************************************
EN_Global_Status RN8209C_Write_Reg(EN_RN8209C_REG_ADDR RegAddr)
{
	u8 tmpData1[RN8209C_BUF_SIZE] = {0};
	u8 tmpData2[RN8209C_BUF_SIZE] = {0};
	u8 Len = 0;
	u8 i = 0;

	if(If_Found_RN8209C_Reg(RegAddr) == false)
	{
		return Status_Error;
	}

	Len = RN8209C_Reg_Value_2_Buf(RegAddr, tmpData1);
	if((Len == 0) || (Len > 4))
	{
		return Status_Error;
	}

	tmpData2[0] = RegAddr | 0x80;

	for(i=0; i<Len; i++)
	{
		tmpData2[i+1] = tmpData1[i];
	}

	Len += 1;

	tmpData2[Len] = (~(Check_Sum(tmpData2, Len)));

	Len += 1;

	RN8209C_Send(tmpData2, Len);

	HAL_Delay(TIME_25MS);

	return Status_Success;
}

//****************************************************
//RN8209C读寄存器
//****************************************************
EN_Global_Status RN8209C_Read_Reg(EN_RN8209C_REG_ADDR RegAddr)
{
	EN_Global_Status Status = Status_Success;

	if(If_Found_RN8209C_Reg(RegAddr) == false)
	{
		return Status_Error;
	}

	//发送要读取的寄存器
	RN8209C_Send((u8*)&RegAddr, 1);

	Status = RN8209C_Wait_Ack(RegAddr);

	return Status;
}




