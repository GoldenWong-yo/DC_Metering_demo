#ifndef _RN8209C_READ_H_
#define _RN8209C_READ_H_

#include "RN8209C_reg.h"





extern ST_RN8209C_DATA_REG RN8209C_AverageData;

typedef struct{
	float U;
	float IA;
	float PA;
	double EA;
	float PA1;
	float EA1;
	float IB;
	float PB;
	double EB;
	float PB1;
	float EB1;
}ST_RN8209C_ANALOG;

extern ST_RN8209C_ANALOG RN8209C_Analog;


#define SAMP_TIMES 25

typedef struct{
	u32 WaitTick;
	u32 WaitTick1;
	ST_RN8209C_DATA_REG DataBuf[SAMP_TIMES];
	u8 DataIdx;
	u32 CalculateEnergyCount;
}ST_RN8209C_READ;

extern ST_RN8209C_READ RN8209C_Read;


void RN8209C_Read_Handler(void);

#endif

