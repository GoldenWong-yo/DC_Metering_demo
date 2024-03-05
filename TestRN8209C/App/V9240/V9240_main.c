#include "V9240_main.h"
#include "V9240_init.h"
#include "V9240_calibrate.h"
#include "V9240_read.h"





typedef struct{
	EN_V9240_MAIN_STATE State;
}ST_V9240;

ST_V9240 V9240;


EN_V9240_MAIN_STATE Get_V9240_Main_State(void)
{
	return V9240.State;
}

void Set_V9240_Main_State(EN_V9240_MAIN_STATE State)
{
	IF_V9240_MAIN_STATE_OK(State);

	V9240.State = State;
}

void V9240_Handler(void)
{
	switch(V9240.State)
	{
		case V9240_MAIN_INIT:
			V9240_Init(V9240_INIT_ALL);
			V9240.State = V9240_MAIN_RUN;
			break;
		case V9240_MAIN_CALIBRATE:
			if(V9240_Calibrate_Handler() == Status_Success)
			{
				V9240.State = V9240_MAIN_INIT;
			}
			break;
		default:
			break;
	}

	V9240_Read_Handler();
}

