#include "V9240_init.h"
#include "V9240_drv.h"
#include "app_storage_V9240.h"





static void V9240_Set_Adc_Samp_Freq(void)
{
	if(V9240_Reg.SSC.SysCtrl.Value_B.ADCLKSEL == ADC_SAMP_FREQ_819200Hz || V9240_Reg.SSC.SysCtrl.Value_B.ADCLKSEL == ADC_SAMP_FREQ_409600Hz)
	{
		V9240_Reg.SSC.SysCtrl.Value_B.CKMDIV = CLK_FREQ_3276800Hz;
	}else{
		V9240_Reg.SSC.SysCtrl.Value_B.CKMDIV = CLK_FREQ_819200Hz;
	}
}

static void V9240_Set_SysClk(void)
{
	//正常计量时，必须保证 ADC 采样频率是电能计量时钟频率的四分之一或八分之一。
	if(V9240_Reg.SSC.SysCtrl.Value_B.CKMDIV == CLK_FREQ_3276800Hz)
	{
		if(V9240_Reg.SSC.SysCtrl.Value_B.ADCLKSEL == ADC_SAMP_FREQ_819200Hz || V9240_Reg.SSC.SysCtrl.Value_B.ADCLKSEL == ADC_SAMP_FREQ_409600Hz)
		{
			//啥也不干
		}else{
			V9240_Reg.SSC.SysCtrl.Value_B.ADCLKSEL = ADC_SAMP_FREQ_819200Hz;
		}
	}else{
		if(V9240_Reg.SSC.SysCtrl.Value_B.ADCLKSEL == ADC_SAMP_FREQ_204800Hz || V9240_Reg.SSC.SysCtrl.Value_B.ADCLKSEL == ADC_SAMP_FREQ_102400Hz)
		{
			//啥也不干
		}else{
			V9240_Reg.SSC.SysCtrl.Value_B.ADCLKSEL = ADC_SAMP_FREQ_204800Hz;
		}
	}
}

static void V9240_Reset(void)
{
	V9240_Reg.SSC.SFTRST = V9240_SFTRST;

	V9240_Write_Reg(Addr_SFTRST);
}

static u32 V9240_Init_SysCtrl(bool Calibrate_Adc_DC_Offset)
{
	u32 CheckSum = 0;

	V9240_Reg.SSC.SysCtrl.Value = 0;
	V9240_Reg.SSC.SysCtrl.Value_B.ADCUPDN = ADCPDN_ACTIVE;
	V9240_Reg.SSC.SysCtrl.Value_B.ADCIAPDN = ADCPDN_ACTIVE;
	V9240_Reg.SSC.SysCtrl.Value_B.BPHPF = DC_METERING;

	//校准adc dc偏置
	if(Calibrate_Adc_DC_Offset)
	{
//		V9240_Reg.SSC.SysCtrl.Value_B.SHORTU = SHORT_ON;
//		V9240_Reg.SSC.SysCtrl.Value_B.SHORTI = SHORT_ON;
	}

	V9240_Write_Reg(Addr_SysCtrl);

	//计算校验
	CheckSum += V9240_Reg.SSC.SysCtrl.Value;

	return CheckSum;
}

static u32 V9240_Init_AnaCtrl_1(void)
{
	u32 CheckSum = 0;

	V9240_Reg.SSC.AnaCtrl1.Value = 0;
	V9240_Reg.SSC.AnaCtrl1.Value_B.CSEL = CSEL_33_PTC;

	V9240_Write_Reg(Addr_AnaCtrl1);

	//计算校验
	CheckSum += V9240_Reg.SSC.AnaCtrl1.Value;

	return CheckSum;
}

static u32 V9240_Init_Adc_DC_Offset(void)
{
	u32 CheckSum = 0;

	V9240_Reg.CP.UADCC = Storage_V9240.CP_Reg.UADCC;
	V9240_Reg.CP.IAADCC = Storage_V9240.CP_Reg.IAADCC;

	V9240_Write_Reg(Addr_UADCC);
	V9240_Write_Reg(Addr_IAADCC);

	//计算校验
	CheckSum += V9240_Reg.CP.UADCC;
	CheckSum += V9240_Reg.CP.IAADCC;

	return CheckSum;
}

static u32 V9240_Init_U_Gain(void)
{
	u32 CheckSum = 0;

	V9240_Reg.CP.UC = Storage_V9240.CP_Reg.UC;

	V9240_Write_Reg(Addr_UC);

	//计算校验
	CheckSum += V9240_Reg.CP.UC;

	return CheckSum;
}

static u32 V9240_Init_U_Offset(void)
{
	u32 CheckSum = 0;

	V9240_Reg.CP.UDCC = Storage_V9240.CP_Reg.UDCC;

	V9240_Write_Reg(Addr_UDCC);

	//计算校验
	CheckSum += V9240_Reg.CP.UDCC;

	return CheckSum;
}

static u32 V9240_Init_IA_Gain(void)
{
	u32 CheckSum = 0;

	V9240_Reg.CP.IAC = Storage_V9240.CP_Reg.IAC;

	V9240_Write_Reg(Addr_IAC);

	//计算校验
	CheckSum += V9240_Reg.CP.IAC;

	return CheckSum;
}

static u32 V9240_Init_IA_Offset(void)
{
	u32 CheckSum = 0;

	V9240_Reg.CP.IADCC = Storage_V9240.CP_Reg.IADCC;

	V9240_Write_Reg(Addr_IADCC);

	//计算校验
	CheckSum += V9240_Reg.CP.IADCC;

	return CheckSum;
}

static u32 V9240_Init_PA_Gain(void)
{
	u32 CheckSum = 0;

	V9240_Reg.CP.PAC = Storage_V9240.CP_Reg.PAC;

	V9240_Write_Reg(Addr_PAC);

	//计算校验
	CheckSum += V9240_Reg.CP.PAC;

	return CheckSum;
}

static u32 V9240_Init_PA_Offset(void)
{
	u32 CheckSum = 0;

	V9240_Reg.CP.PADCC = Storage_V9240.CP_Reg.PADCC;

	V9240_Write_Reg(Addr_PADCC);

	//计算校验
	CheckSum += V9240_Reg.CP.PADCC;

	return CheckSum;
}

static void V9240_Init_CheckSum(void)
{
	V9240_Reg.CP.CKSUM = V9240_CheckSum;

	V9240_Write_Reg(Addr_CKSUM);
}

typedef enum{
	V9240_RESET = 0,
	V9240_SET_SYSCTRL,
	V9240_SET_SYSCTRL_1,		//校准adc dc偏置专用的初始化
	V9240_SET_ANACTRL_1,
	V9240_SET_ADC_DC_OFFSET,	//adc dc偏置
	V9240_SET_U_GAIN,			//电压增益
	V9240_SET_U_OFFSET,
	V9240_SET_IA_GAIN,			//电流增益
	V9240_SET_IA_OFFSET,
	V9240_SET_PA_GAIN,			//功率增益
	V9240_SET_PA_OFFSET,
	V9240_SET_CHECK_SUM,
	V9240_INIT_FUNC_MAX,
}EN_V9240_INIT_FUNC;

#define IF_V9240_INIT_FUNC_OK(Func) if(Func >= V9240_INIT_FUNC_MAX) return


static u32 V9240_Init_Func(EN_V9240_INIT_FUNC Func)
{
	u32 CheckSum = 0;

	IF_V9240_INIT_FUNC_OK(Func) CheckSum;

	switch(Func)
	{
		case V9240_RESET:
			V9240_Reset();
			break;
		case V9240_SET_SYSCTRL:
			CheckSum = V9240_Init_SysCtrl(false);
			break;
		case V9240_SET_SYSCTRL_1:
			CheckSum = V9240_Init_SysCtrl(true);
			break;
		case V9240_SET_ANACTRL_1:
			CheckSum = V9240_Init_AnaCtrl_1();
			break;
		case V9240_SET_ADC_DC_OFFSET:
			CheckSum = V9240_Init_Adc_DC_Offset();
			break;
		case V9240_SET_U_GAIN:
			CheckSum = V9240_Init_U_Gain();
			break;
		case V9240_SET_U_OFFSET:
			CheckSum = V9240_Init_U_Offset();
			break;
		case V9240_SET_IA_GAIN:
			CheckSum = V9240_Init_IA_Gain();
			break;
		case V9240_SET_IA_OFFSET:
			CheckSum = V9240_Init_IA_Offset();
			break;
		case V9240_SET_PA_GAIN:
			CheckSum = V9240_Init_PA_Gain();
			break;
		case V9240_SET_PA_OFFSET:
			CheckSum = V9240_Init_PA_Offset();
			break;
		case V9240_SET_CHECK_SUM:
			V9240_Init_CheckSum();
			break;
		default:
			break;
	}

	return CheckSum;
}

void V9240_Init(EN_V9240_INIT_TYPE Type)
{
	u32 CheckSum = 0;

	IF_V9240_INIT_TYPE_OK(Type);

	CheckSum += V9240_Init_Func(V9240_RESET);
	CheckSum += V9240_Init_Func(V9240_SET_ANACTRL_1);

	switch(Type)
	{
		case V9240_INIT_ALL:
			CheckSum += V9240_Init_Func(V9240_SET_PA_OFFSET);
		case V9240_INIT_CALIBRATE_P_GAIN_OFFSET:
			CheckSum += V9240_Init_Func(V9240_SET_IA_GAIN);
			CheckSum += V9240_Init_Func(V9240_SET_PA_GAIN);
		case V9240_INIT_CALIBRATE_P_GAIN:
		case V9240_INIT_CALIBRATE_I_GAIN:
			CheckSum += V9240_Init_Func(V9240_SET_U_GAIN);
		case V9240_INIT_CALIBRATE_U_GAIN:
			CheckSum += V9240_Init_Func(V9240_SET_U_OFFSET);
			CheckSum += V9240_Init_Func(V9240_SET_IA_OFFSET);
		case V9240_INIT_CALIBRATE_U_I_ZERO:
			CheckSum += V9240_Init_Func(V9240_SET_ADC_DC_OFFSET);
		case V9240_INIT_START:
			CheckSum += V9240_Init_Func(V9240_SET_SYSCTRL);
			break;
		case V9240_INIT_CALIBRATE_ADC_ZERO:
			CheckSum += V9240_Init_Func(V9240_SET_SYSCTRL_1);
			break;
		default:
			break;
	}

	V9240_CheckSum = (~CheckSum);

	CheckSum += V9240_Init_Func(V9240_SET_CHECK_SUM);
}



