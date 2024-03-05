#include "V9240_calibrate.h"
#include "V9240_main.h"
#include "V9240_read.h"
#include "V9240_init.h"
#include "app_storage_V9240.h"
#include "app_storage.h"





typedef enum{
	V9240_CALIBRATE_STATE_1 = 0,
	V9240_CALIBRATE_STATE_2,
	V9240_CALIBRATE_STATE_3,
	V9240_CALIBRATE_STATE_4,
}EN_V9240_CALIBRATE_STATE;

typedef struct{
	u32 WaitTick;
	EN_V9240_CALIBRATE_FUNC Func;
	EN_V9240_CALIBRATE_STATE State;
}ST_V9240_CALIBRATE;

ST_V9240_CALIBRATE V9240_Calibrate;


void V9240_Set_Calibrate_Func(EN_V9240_CALIBRATE_FUNC Func)
{
	IF_V9240_CALIBRATE_FUNC_OK(Func);

	V9240_Calibrate.Func = Func;

	V9240_Calibrate.State = V9240_CALIBRATE_STATE_1;

	Reset_Tick(&V9240_Calibrate.WaitTick);

	Set_V9240_Main_State(V9240_MAIN_CALIBRATE);
}





//校准adc dc偏置
static void V9240_Calibrate_Adc_DC_Offset(void)
{
	Storage_V9240.CP_Reg.UADCC = V9240_Reg.MD.UDCINST;
	Storage_V9240.CP_Reg.IAADCC = V9240_Reg.MD.IADCINST;

	Storage_Set_NeedSave_Flag(STORAGE_V9240);
}

//校准 U 和 I 零点偏移
static void V9240_Calibrate_U_I_Offset(void)
{
	s64 Temp64 = 0;

	Temp64 = V9240_Reg.MD.UAVG;
	Temp64 *= V9240_Reg.MD.UAVG;
	Storage_V9240.CP_Reg.UDCC = (Temp64 >> 8);

	Temp64 = V9240_Reg.MD.IAAVG;
	Temp64 *= V9240_Reg.MD.IAAVG;
	Storage_V9240.CP_Reg.IADCC = (Temp64 >> 8);

	Storage_Set_NeedSave_Flag(STORAGE_V9240);
}

#define V9240_Un 60.0f		//额定电压
#define V9240_Ib 30.0f		//额定电流

#define _2_31_ 2147483648

//校准电压倍差
static void V9240_Calibrate_U_Gain(void)
{
	float Err = 0.0f;

	Err = (V9240_Analog.U - V9240_Un) / V9240_Un;
	Err = Err / (1 + Err);
	Err = 0 - Err;
	Storage_V9240.CP_Reg.UC = Err * _2_31_;

	Storage_Set_NeedSave_Flag(STORAGE_V9240);
}

//校准电流倍差
static void V9240_Calibrate_I_Gain(void)
{
	float Err = 0.0f;

	Err = (V9240_Analog.I - V9240_Ib) / V9240_Ib;
	Err = Err / (1 + Err);
	Err = 0 - Err;
	Storage_V9240.CP_Reg.IAC = Err * _2_31_;

	Storage_Set_NeedSave_Flag(STORAGE_V9240);
}

//校准功率倍差
static void V9240_Calibrate_P_Gain(void)
{
	float Err = 0.0f;
	float TempP = 0.0f;

	TempP = V9240_Analog.U * V9240_Ib;
	Err = (V9240_Analog.P - TempP) / TempP;
	Err = Err / (1 + Err);
	Err = 0 - Err;
	Storage_V9240.CP_Reg.PAC = Err * _2_31_;

	Storage_Set_NeedSave_Flag(STORAGE_V9240);
}

//校准功率偏置
static void V9240_Calibrate_P_Gain_Offset(void)
{
	float Err = 0.0f;
	float TempP = 0.0f;

	TempP = V9240_Analog.U * V9240_Analog.I;
	Err = (V9240_Analog.P - TempP) / TempP;
	Err = Err / (1 + Err);
	Err = 0 - Err;
	Storage_V9240.CP_Reg.PADCC = Err * V9240_Reg.MD.PAAVG;

	Storage_Set_NeedSave_Flag(STORAGE_V9240);
}





#define WAIT_DATA_REFRESH_TIME TIME_5S

static EN_Global_Status V9240_Calibrate_Zero(void)
{
	EN_Global_Status Status = Status_Wait;

	switch(V9240_Calibrate.State)
	{
		case V9240_CALIBRATE_STATE_1:
			V9240_Init(V9240_INIT_CALIBRATE_ADC_ZERO);

			V9240_Calibrate.State = V9240_CALIBRATE_STATE_2;
			break;
		case V9240_CALIBRATE_STATE_2:
			if(Tick_Timeout(&V9240_Calibrate.WaitTick, WAIT_DATA_REFRESH_TIME))
			{
				V9240_Calibrate_Adc_DC_Offset();

				V9240_Init(V9240_INIT_CALIBRATE_U_I_ZERO);

				V9240_Calibrate.State = V9240_CALIBRATE_STATE_3;
			}
			break;
		case V9240_CALIBRATE_STATE_3:
			if(Tick_Timeout(&V9240_Calibrate.WaitTick, WAIT_DATA_REFRESH_TIME))
			{
				V9240_Calibrate_U_I_Offset();

				Status = Status_Success;
			}
			break;
		default:
			break;
	}

	return Status;
}

static EN_Global_Status V9240_Calibrate_U(void)
{
	EN_Global_Status Status = Status_Wait;

	switch(V9240_Calibrate.State)
	{
		case V9240_CALIBRATE_STATE_1:
			V9240_Init(V9240_INIT_CALIBRATE_U_GAIN);

			V9240_Calibrate.State = V9240_CALIBRATE_STATE_2;
			break;
		case V9240_CALIBRATE_STATE_2:
			if(Tick_Timeout(&V9240_Calibrate.WaitTick, WAIT_DATA_REFRESH_TIME))
			{
				V9240_Calibrate_U_Gain();

				Status = Status_Success;
			}
			break;
		default:
			break;
	}

	return Status;
}

static EN_Global_Status V9240_Calibrate_I_And_P(void)
{
	EN_Global_Status Status = Status_Wait;

	switch(V9240_Calibrate.State)
	{
		case V9240_CALIBRATE_STATE_1:
			V9240_Init(V9240_INIT_CALIBRATE_I_GAIN);

			V9240_Calibrate.State = V9240_CALIBRATE_STATE_2;
			break;
		case V9240_CALIBRATE_STATE_2:
			if(Tick_Timeout(&V9240_Calibrate.WaitTick, WAIT_DATA_REFRESH_TIME))
			{
				V9240_Calibrate_I_Gain();
				V9240_Calibrate_P_Gain();

				Status = Status_Success;
			}
			break;
		default:
			break;
	}

	return Status;
}

static EN_Global_Status V9240_Calibrate_P_Offset(void)
{
	EN_Global_Status Status = Status_Wait;

	switch(V9240_Calibrate.State)
	{
		case V9240_CALIBRATE_STATE_1:
			V9240_Init(V9240_INIT_CALIBRATE_P_GAIN_OFFSET);

			V9240_Calibrate.State = V9240_CALIBRATE_STATE_2;
			break;
		case V9240_CALIBRATE_STATE_2:
			if(Tick_Timeout(&V9240_Calibrate.WaitTick, WAIT_DATA_REFRESH_TIME))
			{
				V9240_Calibrate_P_Gain_Offset();

				Status = Status_Success;
			}
			break;
		default:
			break;
	}

	return Status;
}

EN_Global_Status V9240_Calibrate_Handler(void)
{
	EN_Global_Status Status = Status_Wait;

	switch(V9240_Calibrate.Func)
	{
		case V9240_CALIBRATE_FUNC_ZERO:
			Status = V9240_Calibrate_Zero();
			break;
		case V9240_CALIBRATE_FUNC_U_GAIN:
			Status = V9240_Calibrate_U();
			break;
		case V9240_CALIBRATE_FUNC_I_AND_P_GAIN:
			Status = V9240_Calibrate_I_And_P();
			break;
		case V9240_CALIBRATE_FUNC_P_GAIN_OFFSET:
			Status = V9240_Calibrate_P_Offset();
			break;
		default:
			break;
	}

	return Status;
}



