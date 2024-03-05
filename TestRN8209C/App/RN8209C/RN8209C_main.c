#include "RN8209C_main.h"
#include "RN8209C_init.h"
#include "RN8209C_calibrate.h"
#include "RN8209C_read.h"





typedef struct{
	EN_RN8209C_MAIN_STATE State;
	
}ST_RN8209C;

ST_RN8209C RN8209C;


EN_RN8209C_MAIN_STATE Get_RN8209C_Main_State(void)
{
	return RN8209C.State;
}

void Set_RN8209C_Main_State(EN_RN8209C_MAIN_STATE State)
{
	IF_RN8209C_MAIN_STATE_OK(State);

	RN8209C.State = State;
}

void RN8209C_Handler(void)
{
	switch(RN8209C.State)
	{
		case RN8209C_MAIN_INIT:
			RN8209C_Init(RN8209C_INIT_ALL);
			RN8209C.State = RN8209C_MAIN_RUN;
			break;
		case RN8209C_MAIN_CALIBRATE:
			//校准基准的时候，必须要将输入接地
			if(RN8209C_Calibrate_Handler() == Status_Success)
			{
				RN8209C.State = RN8209C_MAIN_INIT;
			}
			break;
		default:
			break;
	}

	RN8209C_Read_Handler();
}

