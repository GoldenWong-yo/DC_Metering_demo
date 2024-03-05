#ifndef _V9240_READ_H_
#define _V9240_READ_H_

#include "app_global.h"





typedef struct{
	float U;
	float I;
	float P;
	float E;
	float P1;
	float E1;
}ST_V9240_ANALOG;

extern ST_V9240_ANALOG V9240_Analog;

typedef struct{
	u32 WaitTick;
	u32 WaitTick1;
	u32 WaitTick2;
	u32 CalculateEnergyCount;
	u32 CalculateEnergyCount1;
}ST_V9240_READ;

extern ST_V9240_READ V9240_Read;


void V9240_Read_Handler(void);

#endif

