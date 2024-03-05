#include "app_math.h"





ST_MATH_VALUE Math_Calculate_Data(EN_CALCULATE_TYPE C_Type, u16 *DataBuf, u16 Len)			//数据样本分析计算
{
	ST_MATH_VALUE MathValue = {0.0f,0.0f,0.0f};
	u16 MaxValue = 0;
	u16 MinValue = 0;
	u16 MaxValueIdx = 0;
	u16 MinValueIdx = 0;
	float TempTotal = 0.0f;
	float TempData = 0.0f;
	float TempOffset = 0.0f;
	float TempVariance = 0.0f;
	u16 Idx;

	if(C_Type >= CALCULATE_TYPE_MAX)
	{
		return MathValue;
	}
	
	if(Len == 0)
	{
		return MathValue;
	}
	
	MaxValue = DataBuf[0];
	MinValue = DataBuf[0];
	
	for(Idx=0; Idx<Len; Idx++)						//选出样本最大值和最小值
	{
		if(MaxValue < DataBuf[Idx])
		{
			MaxValue    = DataBuf[Idx];
			MaxValueIdx = Idx;
		}
		
		if(MinValue > DataBuf[Idx])
		{
			MinValue    = DataBuf[Idx];
			MinValueIdx = Idx;
		}
	}
	
	if(MaxValueIdx == MinValueIdx)					//最大值和最小值相同
	{
		MaxValueIdx++;
	}
	
	for(Idx=0; Idx<Len; Idx++)						//求和
	{
		if(Idx != MaxValueIdx && Idx != MinValueIdx)
		{
			TempTotal += (float)DataBuf[Idx];
		}
	}
	
	MathValue.Average = TempTotal / (Len - 2);		//平均值

	if(C_Type > ONLY_AVERAGE)
	{
		for(Idx=0; Idx<Len; Idx++)
		{
			if(Idx != MaxValueIdx && Idx != MinValueIdx)
			{
				TempData = (float)DataBuf[Idx];
				TempOffset = TempData > MathValue.Average ? (TempData - MathValue.Average) : (MathValue.Average - TempData);
				TempVariance += pow(TempOffset,2);
			}
		}
		
		MathValue.Variance = TempVariance / (Len - 2);	//方差
		
		if(C_Type > ENABLE_VARIANCE)
		{
			MathValue.StandardDeviation = sqrt(MathValue.Variance);
		}
	}
	
	return MathValue;
}




