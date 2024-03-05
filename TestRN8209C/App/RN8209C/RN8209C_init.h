#ifndef _RN8209C_INIT_H_
#define _RN8209C_INIT_H_

#include "app_global.h"





#define RN8209C_EC 3200.0	//电表常数

typedef enum{
	U_ADC_GAIN_1 = 1,		//RN8209C内部 adc 放大器，1倍增益，不增益
	U_ADC_GAIN_2,			//RN8209C内部 adc 放大器，2倍增益
	U_ADC_GAIN_4 = 4,		//RN8209C内部 adc 放大器，4倍增益
}EN_U_ADC_GAIN;				//电压通道增益

#define U_ADC_GAIN U_ADC_GAIN_4

typedef enum{
	IA_ADC_GAIN_1 = 1,		//RN8209C内部 adc 放大器，1倍增益，不增益
	IA_ADC_GAIN_2,			//RN8209C内部 adc 放大器，2倍增益
	IA_ADC_GAIN_8 = 8,		//RN8209C内部 adc 放大器，8倍增益
	IA_ADC_GAIN_16 = 16,	//RN8209C内部 adc 放大器，16倍增益
}EN_IA_ADC_GAIN;			//电流通道增益

#define IA_ADC_GAIN IA_ADC_GAIN_16

#define U_S_Gain 331.0			//电压采样电阻比例
#define I_S_Gain_1 4000.0		//电流采样电阻比例，采样电阻250μΩ，（I_S_Gain_1 = 1 / 250μΩ）
#define I_S_Gain_2 2000.0		//电流采样电阻比例，采样电阻500μΩ，（I_S_Gain_2 = 1 / 500μΩ）
#define I_S_Gain I_S_Gain_2		//电流采样电阻比例

#define R_U (U_ADC_GAIN / U_S_Gain)
#define R_I (IA_ADC_GAIN / I_S_Gain)

#define KP_Coefficient 2147483648.0		//2^31

//功率转换系数Kp
//#define KP_VALUE 0.00061683845399927936102214612458882f
//#define KP_VALUE 0.00061653554439544677734375f
#define KP_VALUE (1 / (R_U * R_I * KP_Coefficient))





typedef enum{
	RN8209C_INIT_CALIBRATE_DC_OFFSET = 0,		//校准dc偏置
	RN8209C_INIT_CALIBRATE_EFFECTIVE_OFFSET,		//校准有效值偏置
	RN8209C_INIT_CALIBRATE_U_I_P_GAIN,			//校准放大系数
	RN8209C_INIT_CALIBRATE_P_GAIN_OFFSET,		//校准功率偏置
	RN8209C_INIT_ALL,
	RN8209C_INIT_TYPE_MAX,
}EN_RN8209C_INIT_TYPE;

#define IF_RN8209C_INIT_TYPE_OK(Type) if(Type >= RN8209C_INIT_TYPE_MAX) return


void RN8209C_Init(EN_RN8209C_INIT_TYPE Type);

#endif

