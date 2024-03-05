#include "V9240_read.h"
#include "V9240_drv.h"
#include "V9240_main.h"
#include "app_storage.h"
#include "app_storage_V9240.h"





#define READ_REG_NUM 6

//#define PARAMETER_POWER 0.0000694444f
//#define PARAMETER_POWER 0.000194444444444444444f
#define CYCLE_TIME_600MS 0.6
#define CYCLE_TIME_200MS 0.2

#define FLOAT_RESOLUTION 1.0

#define U_SIMP_RATIO 331.0		//电压采样比例
#define U_ADC_GAIN 4.0			//电压ADC增益
#define I_SIMP_RATIO 4000.0		//电流采样比例
#define I_ADC_GAIN 32.0			//电流ADC增益
#define U_I_CALCU_PARA 919495302.0		//电压电流计算系数
#define P_CALCU_PARA 709441000.0		//功率计算系数

ST_V9240_ANALOG V9240_Analog;

ST_V9240_READ V9240_Read;


static void V9240_Analog_Check(void)
{
	if(V9240_Analog.U < 0.0f)
	{
		V9240_Analog.U = 0.0f - V9240_Analog.U;
	}

	if(V9240_Analog.I < 0.0f)
	{
		V9240_Analog.I = 0.0f - V9240_Analog.I;
	}

	if(V9240_Analog.P < 0.0f)
	{
		V9240_Analog.P = 0.0f - V9240_Analog.P;
	}
}

static void V9240_Analog_Check1(void)
{
	if(V9240_Analog.P1 < 0.0f)
	{
		V9240_Analog.P1 = 0.0f - V9240_Analog.P1;
	}
}

void V9240_Read_Handler(void)
{
	EN_Global_Status Status[READ_REG_NUM] = {Status_Success};
	u8 i = 0;
	float TempE = 0.0f;

	if(Tick_Timeout(&V9240_Read.WaitTick, TIME_600MS))
	{
		Status[0] = V9240_Read_Reg(Addr_SysSts);
		Status[1] = V9240_Read_Reg(Addr_UAVG);
		Status[2] = V9240_Read_Reg(Addr_IAAVG);
		Status[3] = V9240_Read_Reg(Addr_PAAVG);
		Status[4] = V9240_Read_Reg(Addr_UDCINST);
		Status[5] = V9240_Read_Reg(Addr_IADCINST);

		for(i=0; i<READ_REG_NUM; i++)
		{
			if(Status[i] != Status_Success)
			{
				Set_V9240_Main_State(V9240_MAIN_INIT);
				return;
			}
		}

		if(V9240_Reg.SSC.SysSts.Value_B.BISTERR || V9240_Reg.SSC.SysSts.Value_B.CHKERR)
		{
			Set_V9240_Main_State(V9240_MAIN_INIT);
			return;
		}

		V9240_Analog.U = U_SIMP_RATIO * V9240_Reg.MD.UAVG / (U_I_CALCU_PARA * U_ADC_GAIN);
		V9240_Analog.I = I_SIMP_RATIO * V9240_Reg.MD.IAAVG / (U_I_CALCU_PARA * I_ADC_GAIN);
		V9240_Analog.P = U_SIMP_RATIO * I_SIMP_RATIO * V9240_Reg.MD.PAAVG / (P_CALCU_PARA * U_ADC_GAIN * I_ADC_GAIN);

		V9240_Analog_Check();
	}

	if(V9240_Read.CalculateEnergyCount > 0)
	{
		V9240_Read.CalculateEnergyCount--;

		TempE = V9240_Analog.P * CYCLE_TIME_600MS / 3600;

		Storage_V9240.E.Float += TempE;
		if(Storage_V9240.E.Float >= FLOAT_RESOLUTION)
		{
			Storage_V9240.E.Int   += 1;
			Storage_V9240.E.Float -= FLOAT_RESOLUTION;
		}

		V9240_Analog.E = (float)Storage_V9240.E.Int + Storage_V9240.E.Float;
	}

	if(Tick_Timeout(&V9240_Read.WaitTick1, TIME_200MS))
	{
		Status[0] = V9240_Read_Reg(Addr_PAINST);

		V9240_Analog.P1 = U_SIMP_RATIO * I_SIMP_RATIO * V9240_Reg.MD.PAINST / (P_CALCU_PARA * U_ADC_GAIN * I_ADC_GAIN);

		V9240_Analog_Check1();
	}

	if(V9240_Read.CalculateEnergyCount1 > 0)
	{
		V9240_Read.CalculateEnergyCount1--;

		TempE = V9240_Analog.P1 * CYCLE_TIME_200MS / 3600;

		Storage_V9240.E1.Float += TempE;
		if(Storage_V9240.E1.Float >= FLOAT_RESOLUTION)
		{
			Storage_V9240.E1.Int   += 1;
			Storage_V9240.E1.Float -= FLOAT_RESOLUTION;
		}

		V9240_Analog.E1 = (float)Storage_V9240.E1.Int + Storage_V9240.E1.Float;
	}

	if(Tick_Timeout(&V9240_Read.WaitTick2, TIME_5S))
	{
		Storage_Set_NeedSave_Flag(STORAGE_V9240);
	}
}



