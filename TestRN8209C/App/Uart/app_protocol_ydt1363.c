//**********************************
//铁塔ydt协议
//**********************************

#include "app_protocol_ydt1363.h"
#include "app_responder_ydt.h"
#include "app_tool.h"
#include "app_printf.h"





#define YDT_HEAD 0x7E
#define YDT_END 0x0D

u16 YDT_DataLen = 0;			//协议中数据区的长度


static u16 Check_Sum_YDT(u8* Buf, u16 Len)
{
	return Invert_Check_Sum(Buf, Len);
}





void Pack_Protocol_YDT(EN_DEV_UART DevUart, ST_YDT_INFO ydt_Info)
{
	u16 DataLen = 0;
	u16 Idx = 0;
	u16 CheckSum = 0;
	u16 MoveRightIdx = 0;

	//将数据转换成ascii码，只获取数据区的长度，转换后的数据丢弃，后面重新转换
	ydt_Info.Data.Temp2.Len = Hex2Ascii(ydt_Info.Data.Temp1.pBuf, ydt_Info.Data.Temp1.Len, ydt_Info.Data.Temp2.pBuf);

	//根据长度，计算长度校验后的长度数据
	DataLen = Pack_Sum_Len(ydt_Info.Data.Temp2.Len);

	MoveRightIdx = 6;
	for(Idx=ydt_Info.Data.Temp1.Len-1+MoveRightIdx; Idx>0; Idx--)
	{
		ydt_Info.Data.Temp1.pBuf[Idx] = ydt_Info.Data.Temp1.pBuf[Idx-MoveRightIdx];
	}

	//封装帧头部分
	ydt_Info.Data.Temp1.pBuf[0] = ydt_Info.YDT_Cfg.Ver;
	ydt_Info.Data.Temp1.pBuf[1] = ydt_Info.YDT_Cfg.Addr;

	switch(ydt_Info.YDT_Cfg.Type)
	{
		case YDT_RTN:
			ydt_Info.Data.Temp1.pBuf[2] = CID1_RTN;
			ydt_Info.Data.Temp1.pBuf[3] = ydt_Info.YDT_Cfg.CID2.CID2_RTN;
			break;
		default:
			break;
	}

	//长度字节，先传递高位，再传递低位
	ydt_Info.Data.Temp1.pBuf[4] = (u8)((DataLen & 0xff00) >> 8);
	ydt_Info.Data.Temp1.pBuf[5] = (u8)(DataLen & 0x00ff);
	
	ydt_Info.Data.Temp1.Len += 6;

	//将帧头和数据转换成ascii码
	ydt_Info.Data.Temp2.Len = Hex2Ascii(ydt_Info.Data.Temp1.pBuf, ydt_Info.Data.Temp1.Len, ydt_Info.Data.Temp2.pBuf);

	//计算出校验和
	CheckSum = Check_Sum_YDT(ydt_Info.Data.Temp2.pBuf, ydt_Info.Data.Temp2.Len);

	//封装校验和
	ydt_Info.Data.Temp2.Len += Hex2Ascii(&((u8*)&CheckSum)[1], 1, &ydt_Info.Data.Temp2.pBuf[ydt_Info.Data.Temp2.Len]);
	ydt_Info.Data.Temp2.Len += Hex2Ascii(&((u8*)&CheckSum)[0], 1, &ydt_Info.Data.Temp2.pBuf[ydt_Info.Data.Temp2.Len]);
	
	MoveRightIdx = 1;
	for(Idx=ydt_Info.Data.Temp2.Len-1+MoveRightIdx; Idx>0; Idx--)
	{
		ydt_Info.Data.Temp2.pBuf[Idx] = ydt_Info.Data.Temp2.pBuf[Idx-MoveRightIdx];
	}

	//封装开始码和结束码
	ydt_Info.Data.Temp2.pBuf[YDT_IDX_HEAD] = YDT_HEAD;
	ydt_Info.Data.Temp2.Len += 1;
	
	ydt_Info.Data.Temp2.pBuf[ydt_Info.Data.Temp2.Len] = YDT_END;
	ydt_Info.Data.Temp2.Len += 1;

	Swap_Buf(&ydt_Info.Data.Temp1, &ydt_Info.Data.Temp2);

	Send_Data(DevUart, ydt_Info.Data);
}





bool If_Protocol_YDT(void)
{
	if(UartRcv.Data.Temp1.pBuf[YDT_IDX_HEAD] == YDT_HEAD && UartRcv.Data.Temp1.pBuf[UartRcv.Data.Temp1.Len - 1] == YDT_END)
	{
		return true;
	}

	return false;
}

static u16 Pack_Hex_Buf(u8* InBuf,u8* OutBuf,u16 Len)
{
	OutBuf[YDT_IDX_HEAD] = YDT_HEAD;
	
	memcpy(&OutBuf[YDT_IDX_VER], InBuf, Len);
	
	OutBuf[1+Len] = YDT_END;
	
	return Len+2;
}

void Check_Protocol_YDT(void)
{
	EN_CID2_RTN CheckRTN = CID2_RTN_OK;
	u8 RcvCID2 = 0;				//接收到的CID2信息
	u16 RcvCheckSum = 0;
	u16 TempCheckSum = 0;
	u16 RxDataLen = 0;			//接收到的数据区的长度
	ST_YDT_INFO ydt_Info;

	CheckRTN = CID2_RTN_OK;

	//先计算校验备用
	TempCheckSum = Check_Sum_YDT(&UartRcv.Data.Temp1.pBuf[YDT_IDX_VER], UartRcv.Data.Temp1.Len - 6);

	//将协议由ascii码，转换为hex，头码和尾码不需要转换，长度减2
	UartRcv.Data.Temp2.Len = Ascii2Hex(&UartRcv.Data.Temp1.pBuf[YDT_IDX_VER], UartRcv.Data.Temp1.Len-2, UartRcv.Data.Temp2.pBuf);

	//把hex版本的协议，重新封装好，方便后面的解析
	UartRcv.Data.Temp1.Len = Pack_Hex_Buf(UartRcv.Data.Temp2.pBuf, UartRcv.Data.Temp1.pBuf, UartRcv.Data.Temp2.Len);

	//获取当前的命令是回复命令，还是接收命令
	RcvCID2 = UartRcv.Data.Temp1.pBuf[YDT_IDX_CID2];

	RcvCheckSum  = UartRcv.Data.Temp1.pBuf[UartRcv.Data.Temp1.Len - 3] << 8;
	RcvCheckSum += UartRcv.Data.Temp1.pBuf[UartRcv.Data.Temp1.Len - 2];

	//如果校验通过，将不再计算
	if(RcvCheckSum == TempCheckSum)
	{
		RxDataLen  = UartRcv.Data.Temp1.pBuf[YDT_IDX_LEN_H] << 8;
		RxDataLen += UartRcv.Data.Temp1.pBuf[YDT_IDX_LEN_L];

		//检查协议中的长度描述字段，校验是否正确
		if(Check_Sum_Len(RxDataLen))
		{
			RxDataLen = RxDataLen & 0x0fff;

			//长度值，要和收到的真的数据长度匹配
			if(RxDataLen == (UartRcv.Data.Temp1.Len - YDT_IDX_DATA - 3) * 2)
			{
				YDT_DataLen = UartRcv.Data.Temp1.Len - YDT_IDX_DATA - 3;

				//如果CID2小于RTN代码的最大值，证明当前是个回复命令
				if(RcvCID2 < CID2_RTN_OTHER)
				{
					if(RcvCID2 == CID2_RTN_OK)
					{
						//CheckRTN = Parse_Switch_Ack();
					}
				}else{
					CheckRTN = Parse_Responder_ydt();
				}
			}else{
				CheckRTN = CID2_RTN_LEN_ERROR;
			}
		}else{
			CheckRTN = CID2_RTN_LEN_CHECKSUM_ERROR;
		}
	}else{
		CheckRTN = CID2_RTN_CHECKSUM_ERROR;
	}

	if(RcvCID2 < CID2_RTN_OTHER)
	{
		if(CheckRTN != CID2_RTN_OK || RcvCID2 != CID2_RTN_OK)
		{
			//Switch_Fail_Handler();
		}
	}else{
		if(CheckRTN == CID2_RTN_ADDR_ERROR)
		{
			//一个主机下，挂载多个同种设备时，不能反回地址错误
		}
		else if(CheckRTN == CID2_RTN_CID1_ERROR)
		{
			//一个主机下，挂载多个不同设备时，如果各设备的地址相同，不能返回CID1错误
		}else{
			if(CheckRTN != CID2_RTN_OK)
			{
				UartRcv.Data.Temp1.Len = 0;
			}else{
				//Set_Communicate_Online();
			}

			ydt_Info.Data = UartRcv.Data;

			ydt_Info.YDT_Cfg.Ver  = DEV_VER;
			ydt_Info.YDT_Cfg.Addr = DEV_ADDR;
			ydt_Info.YDT_Cfg.Type = YDT_RTN;
			ydt_Info.YDT_Cfg.CID2.CID2_RTN = CheckRTN;

			Pack_Protocol_YDT(UartRcv.DevUart, ydt_Info);
		}
	}
}




