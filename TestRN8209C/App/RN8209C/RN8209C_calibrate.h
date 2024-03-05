#ifndef _RN8209C_CALIBRATE_H_
#define _RN8209C_CALIBRATE_H_

#include "app_global.h"




//对外显示的校准功能
typedef enum{
	RN8209C_CALIBRATE_FUNC_ZERO = 0,			//校准 U 和 I 零点偏移（0v电压，0a电流）输入必须接地
	RN8209C_CALIBRATE_FUNC_U_GAIN,			//校准电压倍差（48v电压，0a电流）
	RN8209C_CALIBRATE_FUNC_I_AND_P_GAIN,		//校准电流和功率倍差（48v电压，100a电流）
	RN8209C_CALIBRATE_FUNC_P_GAIN_OFFSET,	//校准功率偏移（48v电压，5a电流）
	RN8209C_CALIBRATE_FUNC_MAX,
}EN_RN8209C_CALIBRATE_FUNC;

#define IF_RN8209C_CALIBRATE_FUNC_OK(Func) if(Func >= RN8209C_CALIBRATE_FUNC_MAX) return


void RN8209C_Set_Calibrate_Func(EN_RN8209C_CALIBRATE_FUNC Func);
EN_Global_Status RN8209C_Calibrate_Handler(void);

#endif

