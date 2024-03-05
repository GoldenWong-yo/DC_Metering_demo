#include "V9240_reg.h"





//用于计算校验
u32 V9240_CheckSum = 0;

ST_V9240_REG V9240_Reg;


bool If_Found_V9240_Reg(EN_V9240_REG_ADDR RegAddr)
{
	bool FoundReg = false;

	switch(RegAddr)
	{
		case Addr_SysSts:	//系统状态寄存器
		case Addr_SysCtrl:	//系统控制寄存器
		case Addr_AnaCtrl0:	//模拟控制寄存器0
		case Addr_AnaCtrl1:	//模拟控制寄存器1
		case Addr_SysStsClr://系统状态清除寄存器
		case Addr_SFTRST:	//软件复位寄存器，软件复位寄存器	写入0x4572BEAF，复位所有电路
		
		case Addr_FREQINST:	//瞬时频率值
		case Addr_PAINST:	//A路瞬时有功功率值
		case Addr_QINST:	//瞬时无功功率值
		case Addr_IAINST:	//A路瞬时电流有效值
		case Addr_UINST:	//瞬时电压有效值
		case Addr_PAAVG:	//A路平均有功功率值
		case Addr_QAVG:		//平均无功功率值
		case Addr_FRQAVG:	//平均频率值
		case Addr_IAAVG:	//A路平均电流有效值
		case Addr_UAVG:		//平均电压有效值
		case Addr_PBINST:	//B路瞬时有功功率值
		case Addr_IBINST:	//B路瞬时电流有效值
		case Addr_PBAVG:	//B路平均有功功率值
		case Addr_IBAVG:	//B路平均电流有效值
		case Addr_UDCINST:	//电压瞬时直流分量值
		case Addr_IADCINST:	//A路瞬时直流分量值
		case Addr_IBDCINST:	//B路瞬时直流分量值
		case Addr_ZXDATREG:	//电压过零点前一次采样值
		case Addr_ZXDAT:	//电压过零点当前点采样值
		case Addr_PHDAT:	//电压相位数据
		case Addr_T8BAUD:	//当前波特率通信时连续8bit下降沿时间数据
		
		case Addr_PAC:		//A通道有功功率比差校正
		case Addr_PHC:		//有功功率角差校正
		case Addr_PADCC:	//A通道有功功率小信号校正
		case Addr_QAC:		//A路无功功率比差校正
		case Addr_QBC:		//B路无功功率比差校正
		case Addr_QADCC:	//A路无功功率小信号校正
		case Addr_QBDCC:	//B路无功功率小信号校正
		case Addr_IAC:		//A路电流有效值比差校正
		case Addr_IADCC:	//A路电流有效值小信号校正
		case Addr_UC:		//电压有效值比差校正
		case Addr_PBC:		//B路有功功率比差校正
		case Addr_PBDCC:	//B路有功功率小信号校正
		case Addr_IBC:		//B路电流有效值比差校正
		case Addr_IBDCC:	//B路电流有效值小信号校正
		case Addr_IAADCC:	//A路电流ADC直流偏置校正
		case Addr_IBADCC:	//B路电流ADC直流偏置校正
		case Addr_UADCC:	//电压ADC直流偏置校正
		case Addr_BPFPARA:	//带通滤波器系数
		case Addr_UDCC:		//电压有效值小信号校正
		case Addr_CKSUM:	//校验和寄存器
			FoundReg = true;
			break;
		default:
			break;
	}

	return FoundReg;
}

u32 V9240_Reg_Value_2_Buf(EN_V9240_REG_ADDR RegAddr, u8 Data[])
{
	u32 Len = 0;

	if(If_Found_V9240_Reg(RegAddr) == false)
	{
		return Len;
	}

	switch(RegAddr)
	{
		case Addr_SysCtrl:	//系统控制寄存器
			Len = 4;
			memcpy(Data, &V9240_Reg.SSC.SysCtrl.Value, Len);
			break;
		case Addr_AnaCtrl0:	//模拟控制寄存器0
			Len = 4;
			memcpy(Data, &V9240_Reg.SSC.AnaCtrl0, Len);
			break;
		case Addr_AnaCtrl1:	//模拟控制寄存器1
			Len = 4;
			memcpy(Data, &V9240_Reg.SSC.AnaCtrl1, Len);
			break;
		case Addr_SysStsClr://系统状态清除寄存器
			Len = 4;
			memcpy(Data, &V9240_Reg.SSC.SysStsClr, Len);
			break;
		case Addr_SFTRST:	//软件复位寄存器
			Len = 4;
			memcpy(Data, &V9240_Reg.SSC.SFTRST, Len);
			break;

		case Addr_PAC:		//A通道有功功率比差校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.PAC, Len);
			break;
		case Addr_PHC:		//有功功率角差校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.PHC, Len);
			break;
		case Addr_PADCC:	//A通道有功功率小信号校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.PADCC, Len);
			break;
		case Addr_QAC:		//A路无功功率比差校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.QAC, Len);
			break;
		case Addr_QBC:		//B路无功功率比差校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.QBC, Len);
			break;
		case Addr_QADCC:	//A路无功功率小信号校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.QADCC, Len);
			break;
		case Addr_QBDCC:	//B路无功功率小信号校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.QBDCC, Len);
			break;
		case Addr_IAC:		//A路电流有效值比差校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.IAC, Len);
			break;
		case Addr_IADCC:	//A路电流有效值小信号校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.IADCC, Len);
			break;
		case Addr_UC:		//电压有效值比差校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.UC, Len);
			break;
		case Addr_PBC:		//B路有功功率比差校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.PBC, Len);
			break;
		case Addr_PBDCC:	//B路有功功率小信号校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.PBDCC, Len);
			break;
		case Addr_IBC:		//B路电流有效值比差校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.IBC, Len);
			break;
		case Addr_IBDCC:	//B路电流有效值小信号校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.IBDCC, Len);
			break;
		case Addr_IAADCC:	//A路电流ADC直流偏置校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.IAADCC, Len);
			break;
		case Addr_IBADCC:	//B路电流ADC直流偏置校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.IBADCC, Len);
			break;
		case Addr_UADCC:	//电压ADC直流偏置校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.UADCC, Len);
			break;
		case Addr_BPFPARA:	//带通滤波器系数
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.BPFPARA, Len);
			break;
		case Addr_UDCC:		//电压有效值小信号校正
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.UDCC, Len);
			break;
		case Addr_CKSUM:	//校验和寄存器
			Len = 4;
			memcpy(Data, &V9240_Reg.CP.CKSUM, Len);
			break;
		default:
			break;
	}

	return Len;
}

EN_Global_Status V9240_Parse_Cmd(EN_V9240_REG_ADDR RegAddr, u8 Data[], u8 Len)
{
	EN_Global_Status Status = Status_Success;
	u32 TempReg = 0;

	if(If_Found_V9240_Reg(RegAddr) == false)
	{
		return Status_Error;
	}

	if((Len == 0) || (Len > 4))
	{
		return Status_Error;
	}

	memcpy(&TempReg, Data, Len);

	switch(RegAddr)
	{
		case Addr_SysSts:	//系统状态寄存器
			V9240_Reg.SSC.SysSts.Value = TempReg;
			break;

		case Addr_FREQINST:	//瞬时频率值
			V9240_Reg.MD.FREQINST = TempReg;
			break;
		case Addr_PAINST:	//A路瞬时有功功率值
			V9240_Reg.MD.PAINST = TempReg;
			break;
		case Addr_QINST:	//瞬时无功功率值
			V9240_Reg.MD.QINST = TempReg;
			break;
		case Addr_IAINST:	//A路瞬时电流有效值
			V9240_Reg.MD.IAINST = TempReg;
			break;
		case Addr_UINST:	//瞬时电压有效值
			V9240_Reg.MD.UINST = TempReg;
			break;
		case Addr_PAAVG:	//A路平均有功功率值
			V9240_Reg.MD.PAAVG = TempReg;
			break;
		case Addr_QAVG:		//平均无功功率值
			V9240_Reg.MD.QAVG = TempReg;
			break;
		case Addr_FRQAVG:	//平均频率值
			V9240_Reg.MD.FRQAVG = TempReg;
			break;
		case Addr_IAAVG:	//A路平均电流有效值
			V9240_Reg.MD.IAAVG = TempReg;
			break;
		case Addr_UAVG:		//平均电压有效值
			V9240_Reg.MD.UAVG = TempReg;
			break;
		case Addr_PBINST:	//B路瞬时有功功率值
			V9240_Reg.MD.PBINST = TempReg;
			break;
		case Addr_IBINST:	//B路瞬时电流有效值
			V9240_Reg.MD.IBINST = TempReg;
			break;
		case Addr_PBAVG:	//B路平均有功功率值
			V9240_Reg.MD.PBAVG = TempReg;
			break;
		case Addr_IBAVG:	//B路平均电流有效值
			V9240_Reg.MD.IBAVG = TempReg;
			break;
		case Addr_UDCINST:	//电压瞬时直流分量值
			V9240_Reg.MD.UDCINST = TempReg;
			break;
		case Addr_IADCINST:	//A路瞬时直流分量值
			V9240_Reg.MD.IADCINST = TempReg;
			break;
		case Addr_IBDCINST:	//B路瞬时直流分量值
			V9240_Reg.MD.IBDCINST = TempReg;
			break;
		case Addr_ZXDATREG:	//电压过零点前一次采样值
			V9240_Reg.MD.ZXDATREG = TempReg;
			break;
		case Addr_ZXDAT:	//电压过零点当前点采样值
			V9240_Reg.MD.ZXDAT = TempReg;
			break;
		case Addr_PHDAT:	//电压相位数据
			V9240_Reg.MD.PHDAT = TempReg;
			break;
		case Addr_T8BAUD:	//当前波特率通信时连续8bit下降沿时间数据
			V9240_Reg.MD.T8BAUD = TempReg;
			break;
		default:
			break;
	}

	return Status;
}



