#ifndef _APP_MATH_H_
#define _APP_MATH_H_

#include "app_global.h"




typedef struct{
	float Average;				//平均值
	float Variance;				//方差
	float StandardDeviation;	//标准差
}ST_MATH_VALUE;

typedef enum{
	ONLY_AVERAGE = 0,
	ENABLE_VARIANCE,
	ENABLE_STD_DEVIATION,
	CALCULATE_TYPE_MAX,
}EN_CALCULATE_TYPE;


ST_MATH_VALUE Math_Calculate_Data(EN_CALCULATE_TYPE C_Type, u16 *DataBuf, u16 Len);

#endif

