#ifndef _V9240_CALIBRATE_H_
#define _V9240_CALIBRATE_H_

#include "app_global.h"




//对外显示的校准功能
typedef enum{
	V9240_CALIBRATE_FUNC_ZERO = 0,			//校准adc dc偏置，校准 U 和 I 零点偏移（0v电压，0a电流）
	V9240_CALIBRATE_FUNC_U_GAIN,			//校准电压倍差（48v电压，0a电流）
	V9240_CALIBRATE_FUNC_I_AND_P_GAIN,		//校准电流和功率倍差（48v电压，100a电流）
	V9240_CALIBRATE_FUNC_P_GAIN_OFFSET,		//校准功率偏移（48v电压，5a电流）
	V9240_CALIBRATE_FUNC_MAX,
}EN_V9240_CALIBRATE_FUNC;

#define IF_V9240_CALIBRATE_FUNC_OK(Func) if(Func >= V9240_CALIBRATE_FUNC_MAX) return


void V9240_Set_Calibrate_Func(EN_V9240_CALIBRATE_FUNC Func);
EN_Global_Status V9240_Calibrate_Handler(void);


#endif

