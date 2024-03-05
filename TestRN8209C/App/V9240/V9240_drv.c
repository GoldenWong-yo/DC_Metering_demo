#include "V9240_drv.h"
#include "app_tool.h"
#include "usart.h"





#define V9240_BUF_SIZE 10

#define V9240_P_HEAD 0x7d

typedef enum{
	CTL_READ = 1,
	CTL_WRITE,
}EN_RW_CTL;

#define ONCE_READ_REG_NUM 1

//****************************************************
//V9240发送数据
//****************************************************
static void V9240_Send(u8 Data[], u8 Len)
{
	HAL_GPIO_WritePin(GPIOA, RN8209C_CH_SW_A_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, RN8209C_CH_SW_B_Pin, GPIO_PIN_SET);

	huart3.Init.BaudRate = 19200;
	huart3.Init.Parity = UART_PARITY_ODD;
	if (HAL_UART_Init(&huart3) != HAL_OK)
	{
		Error_Handler();
	}

	memcpy(Uart3.Tx.pBuf, Data, Len);
	Uart3.Tx.Len = Len;

	HAL_UART_AbortReceive(&huart3);
	HAL_UART_Transmit_DMA(&huart3, Uart3.Tx.pBuf, Uart3.Tx.Len);
}

static u8 V9240_Check_Sum(u8 Data[], u32 Len)
{
	u8 CheckSum = 0;

	CheckSum = Check_Sum(Data, Len);
	CheckSum = (~CheckSum);
	CheckSum += 0x33;

	return CheckSum;
}

static EN_Global_Status V9240_Check_Ack_Pack(EN_RW_CTL RW_Ctl, EN_V9240_REG_ADDR RegAddr, u8 Data[], u8 Len)
{
	EN_Global_Status Status = Status_Error;
	u8 TempCSM = 0;

	Len -= 1;

	TempCSM = V9240_Check_Sum(Data, Len);

	if(Data[Len] == TempCSM)
	{
		if(Data[0] == V9240_P_HEAD)
		{
			if(Data[1] == ((RegAddr & 0x0f00) >> 4) + RW_Ctl)
			{
				if(RW_Ctl == CTL_READ)
				{
					if(Data[2] == ONCE_READ_REG_NUM)
					{
						Status = Status_Success;
					}
				}else{
					if(Data[2] == (RegAddr & 0x00ff))
					{
						Status = Status_Success;
					}
				}
			}
		}
	}

	return Status;
}

static EN_Global_Status V9240_Wait_Ack(EN_RW_CTL RW_Ctl, EN_V9240_REG_ADDR RegAddr)
{
	EN_Global_Status Status = Status_Success;
	u32 WaitTick = 0;
	u8 tmpData[V9240_BUF_SIZE] = {0};
	u32 Len = 0;

	Reset_Tick(&WaitTick);

	while(1)
	{
		if(Uart3.Rx.Len > 0)
		{
			memcpy(tmpData, Uart3.Rx.pBuf, Uart3.Rx.Len);
			Len = Uart3.Rx.Len;

			Uart3.Rx.Len = 0;

			Status = V9240_Check_Ack_Pack(RW_Ctl, RegAddr, tmpData, Len);
			if(RW_Ctl == CTL_READ && Status == Status_Success)
			{
				Status = V9240_Parse_Cmd(RegAddr, &tmpData[3], (Len-4));
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
//V9240写寄存器
//****************************************************
EN_Global_Status V9240_Write_Reg(EN_V9240_REG_ADDR RegAddr)
{
	EN_Global_Status Status = Status_Success;
	u8 tmpData1[V9240_BUF_SIZE] = {0};
	u8 tmpData2[V9240_BUF_SIZE] = {0};
	u32 Len = 0;

	if(If_Found_V9240_Reg(RegAddr) == false)
	{
		return Status_Error;
	}

	Len = V9240_Reg_Value_2_Buf(RegAddr, tmpData1);
	if(Len == 0 || Len > 4)
	{
		return Status_Error;
	}

	tmpData2[0] = V9240_P_HEAD;
	tmpData2[1] = ((RegAddr & 0x0f00) >> 4) + CTL_WRITE;
	tmpData2[2] = (RegAddr & 0x00ff);

	memcpy(&tmpData2[3], tmpData1, Len);

	Len += 3;

	tmpData2[Len] = V9240_Check_Sum(tmpData2, Len);

	Len += 1;

	V9240_Send(tmpData2, Len);

	Status = V9240_Wait_Ack(CTL_WRITE, RegAddr);

	return Status;
}

//****************************************************
//V9240读寄存器
//****************************************************
EN_Global_Status V9240_Read_Reg(EN_V9240_REG_ADDR RegAddr)
{
	EN_Global_Status Status = Status_Success;
	u8 tmpData[V9240_BUF_SIZE] = {0};

	if(If_Found_V9240_Reg(RegAddr) == false)
	{
		return Status_Error;
	}

	tmpData[0] = V9240_P_HEAD;
	tmpData[1] = ((RegAddr & 0x0f00) >> 4) + CTL_READ;
	tmpData[2] = (RegAddr & 0x00ff);
	tmpData[3] = ONCE_READ_REG_NUM;
	tmpData[4] = 0;
	tmpData[5] = 0;
	tmpData[6] = 0;
	tmpData[7] = V9240_Check_Sum(tmpData, 7);

	//发送要读取的寄存器
	V9240_Send(tmpData, 8);

	Status = V9240_Wait_Ack(CTL_READ, RegAddr);

	return Status;
}




