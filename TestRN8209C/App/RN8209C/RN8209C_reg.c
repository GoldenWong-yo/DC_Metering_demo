#include "RN8209C_reg.h"





//用于计算校验
u16 RN8209C_CheckSum = 0;

//RN8209C读取和写入的缓存区
ST_RN8209C_REG RN8209C_Reg;


bool If_Found_RN8209C_Reg(EN_RN8209C_REG_ADDR RegAddr)
{
	bool FoundReg = false;

	switch(RegAddr)
	{
		case ADDR_SYSCON:
		case ADDR_EMUCON:
		case ADDR_HFConst:
		case ADDR_PStart:
		case ADDR_DStart:
		case ADDR_GPQA:
		case ADDR_GPQB:
		case ADDR_PhsA:
		case ADDR_PhsB:
		case ADDR_QPHSCAL:
		case ADDR_APOSA:
		case ADDR_APOSB:
		case ADDR_RPOSA:
		case ADDR_RPOSB:
		case ADDR_IARMSOS:
		case ADDR_IBRMSOS:
		case ADDR_IBGain:
		case ADDR_D2FPL:
		case ADDR_D2FPH:
		case ADDR_DCIAH:
		case ADDR_DCIBH:
		case ADDR_DCUH:
		case ADDR_DCL:
		case ADDR_EMUCON2:

		case ADDR_PFCnt:
		case ADDR_DFcnt:
		case ADDR_IARMS:
		case ADDR_IBRMS:
		case ADDR_URMS:
		case ADDR_UFreq:
		case ADDR_PowerPA:
		case ADDR_PowerPB:
		case ADDR_PowerQ:
		case ADDR_EnergyP:
		case ADDR_EnergyP2:
		case ADDR_EnergyD:
		case ADDR_EnergyD2:
		case ADDR_EMUStatus:
		case ADDR_SPL_IA:
		case ADDR_SPL_IB:
		case ADDR_SPL_U:
		case ADDR_UFreq2:

		case ADDR_IE:
		case ADDR_IF:
		case ADDR_RIF:

		case ADDR_SysStatus:
		case ADDR_RData:
		case ADDR_WData:
		case ADDR_DeviceID:
		case ADDR_SpecialFunc:
			FoundReg =  true;
			break;
		default:
			break;
	}

	return FoundReg;
}

static void RN8209C_Swap_Data(u8 S_Data[], u8 Len, u8 O_Data[])
{
	u8 i = 0;

	for(i=0; i<Len; i++)
	{
		O_Data[Len-1-i] = S_Data[i];
	}
}

u8 RN8209C_Reg_Value_2_Buf(EN_RN8209C_REG_ADDR RegAddr, u8 Data[])
{
	u8 Len = 0;

	if(If_Found_RN8209C_Reg(RegAddr) == false)
	{
		return Len;
	}

	switch(RegAddr)
	{
		case ADDR_SYSCON:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.SYSCON.Value, Len, Data);
			break;
		case ADDR_EMUCON:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.EMUCON.Value, Len, Data);
			break;
		case ADDR_HFConst:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.HFConst, Len, Data);
			break;
		case ADDR_PStart:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.PStart, Len, Data);
			break;
		case ADDR_DStart:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.DStart, Len, Data);
			break;
		case ADDR_GPQA:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.GPQA, Len, Data);
			break;
		case ADDR_GPQB:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.GPQB, Len, Data);
			break;
		case ADDR_PhsA:
			Len = 1;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.PhsA, Len, Data);
			break;
		case ADDR_PhsB:
			Len = 1;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.PhsB, Len, Data);
			break;
		case ADDR_QPHSCAL:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.QPHSCAL, Len, Data);
			break;
		case ADDR_APOSA:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.APOSA, Len, Data);
			break;
		case ADDR_APOSB:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.APOSB, Len, Data);
			break;
		case ADDR_RPOSA:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.RPOSA, Len, Data);
			break;
		case ADDR_RPOSB:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.RPOSB, Len, Data);
			break;
		case ADDR_IARMSOS:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.IARMSOS, Len, Data);
			break;
		case ADDR_IBRMSOS:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.IBRMSOS, Len, Data);
			break;
		case ADDR_IBGain:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.IBGain, Len, Data);
			break;
		case ADDR_D2FPL:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.D2FPL, Len, Data);
			break;
		case ADDR_D2FPH:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.D2FPH, Len, Data);
			break;
		case ADDR_DCIAH:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.DCIAH, Len, Data);
			break;
		case ADDR_DCIBH:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.DCIBH, Len, Data);
			break;
		case ADDR_DCUH:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.DCUH, Len, Data);
			break;
		case ADDR_DCL:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.DCL, Len, Data);
			break;
		case ADDR_EMUCON2:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Ctrl.EMUCON2.Value, Len, Data);
			break;
		case ADDR_PFCnt:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Data.PFCnt, Len, Data);
			break;
		case ADDR_DFcnt:
			Len = 2;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.Data.DFcnt, Len, Data);
			break;
		case ADDR_SpecialFunc:
			Len = 1;
			RN8209C_Swap_Data((u8*)&RN8209C_Reg.SSC.SpecialFunc, Len, Data);
			break;
		default:
			break;
	}

	return Len;
}

EN_Global_Status RN8209C_Parse_Cmd(EN_RN8209C_REG_ADDR RegAddr, u8 Data[], u8 Len)
{
	EN_Global_Status Status = Status_Success;
	u32 TempReg = 0;
	u8 Idx = 0;

	if(If_Found_RN8209C_Reg(RegAddr) == false)
	{
		return Status_Error;
	}

	if((Len == 0) || (Len > 4))
	{
		return Status_Error;
	}

	for(Idx=0; Idx<Len; Idx++)
	{
		TempReg = (TempReg | (Data[Idx] << ((Len - 1 - Idx) * 0x08)));
	}

	//电压电流只为正数
	if(RegAddr == ADDR_URMS || RegAddr == ADDR_IARMS || RegAddr == ADDR_IBRMS)
	{
		if(TempReg & 0x00800000)
		{
			TempReg = 0;
		}
	}

	switch(RegAddr)
	{
		case ADDR_URMS:
			RN8209C_Reg.Data.URMS = TempReg;
			break;
		case ADDR_IARMS:
			RN8209C_Reg.Data.IARMS = TempReg;
			break;
		case ADDR_PowerPA:
			RN8209C_Reg.Data.PowerPA = TempReg;
			//RN8209C_Reg.Data.PowerPA = 0xffffffff;
			break;
		case ADDR_EnergyP:
			RN8209C_Reg.Data.EnergyP = TempReg;
			break;
		case ADDR_IBRMS:
			RN8209C_Reg.Data.IBRMS = TempReg;
			break;
		case ADDR_PowerPB:
			RN8209C_Reg.Data.PowerPB = TempReg;
			break;
		case ADDR_EnergyD:
			RN8209C_Reg.Data.EnergyD = TempReg;
			break;
		case ADDR_EMUStatus:
			RN8209C_Reg.Data.EMUStatus = TempReg;
			break;
		case ADDR_PFCnt:
			RN8209C_Reg.Data.PFCnt = TempReg;
			break;
		case ADDR_DFcnt:
			RN8209C_Reg.Data.DFcnt = TempReg;
			break;
		default:
			break;
	}

	return Status;
}



