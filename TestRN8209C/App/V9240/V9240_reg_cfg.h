#ifndef _V9240_REG_CFG_H_
#define _V9240_REG_CFG_H_

#include "app_global.h"





typedef enum{
	PARA_SELF_CHECK_OK = 0,	//参数自检成功
	PARA_SELF_CHECK_FAIL	//参数自检失败
}EN_CHKERR;

typedef enum{
	POR_RST = 0x01,			//系统发生 POR 复位
	RSV_RST = 0x02,			//保留。
	RX_RST = 0x03,			//系统发生 RX 复位。
	SFT_RST = 0x04,			//系统发生软件复位
}EN_RSTSRC;

typedef enum{
	RAM_SELF_CHECK_OK = 0,	//RAM自检成功
	RAM_SELF_CHECK_FAIL 	//RAM自检失败
}EN_BISTERR;

typedef enum{
	U_SIGN_PLUS = 0,		//正号
	U_SIGN_MINUS			//负号
}EN_USIGN;

//=============================================================
//V9240 状态寄存器  
//=============================================================
typedef struct{
	u32 REF:1;					//当 REF 外接电容开始漏电时，该位置高，否则该位为低。
	u32 PHSDONE:1;				//相位测量是否结束标识位。0：未结束；1：已结束。
	EN_CHKERR CHKERR:1;			//参数自检错误中断标志位。读出值每 5ms 刷新一次。对该位进行读操作判断是否发生参数自检错误中断。如果参数配置自检校验通过，该位读出值为 0；否则，自检失败，该位读出值为 1。
	EN_RSTSRC RSTSRC:3;			//复位原因标志位
	u32 PDN_R:1;				//掉电中断标志位锁存值。当系统发生掉电时，即 VDD33 引脚上的电平低于掉电阈值时，该位读出值为 1。当掉电事件消失后，该标志位读出值保持为 1。
	u32 PDN:1;					//掉电中断标志位。当系统发生掉电时，即 VDD33 引脚上的电平低于掉电阈值时，该位读出值为 1。当掉电事件消失后，该标志位读出值为 0。
	EN_BISTERR BISTERR:1;		//系统发生复位后，RAM 即进行自检，耗时约 1.25ms。如果该位读出值为 1，表明 RAM 自检失败。此时，用户应重新复位系统。
	u32 PHSDONE_R:1;			//相位测量是否结束标识位的锁存值。0：未结束；1：已结束
	u32 Reserve_Bit10:1;
	EN_USIGN USIGN:1;			//对该位进行读操作判断电压信号符号。该标志位不可清空，会跟随电压符号位翻转。1：负号；0：正号
	u32 Reserve_Bit12_31:20;
}ST_V9240_SS_B;

typedef union{
	u32 Value;
	ST_V9240_SS_B Value_B;
}UN_V9240_SS;        //System Status





typedef enum{
	U_REG_GAIN_1 = 0,	//电压数字增益 1倍
	U_REG_GAIN_4,		//电压数字增益 4倍
}EN_PGAU_;

typedef enum{
	AC_METERING = 0,	//交流计量场景
	DC_METERING,		//直流计量场景
}EN_BPHPF;

typedef enum{
	U_UNSIGNED = 0, 	//电压符号位不使能
	U_SIGNED,			//电压符号位使能
}EN_IESUL;

typedef enum{
	AC_FREQ_50Hz = 0,	//交流基波50Hz
	AC_FREQ_60Hz,		//交流基波60Hz
}EN_RCX12;

typedef enum{
	SHORT_NULL = 0, 	//不动作
	SHORT_ON,			//将通道放大器端短路，用于获取ADC 自身引入的直流偏置，（直流计量初始化时候需要打开，正常计量时候关闭）
}EN_SHORT;

typedef enum{
	CLK_FREQ_3276800Hz = 0,
	CLK_FREQ_819200Hz,
}EN_CKMDIV;

typedef enum{
	ADC_SAMP_FREQ_819200Hz = 0,
	ADC_SAMP_FREQ_409600Hz,
	ADC_SAMP_FREQ_204800Hz,
	ADC_SAMP_FREQ_102400Hz,
}EN_ADCLKSEL;

typedef enum{
	I_ADC_GAIN_32 = 0,	//I通道adc 32倍增益
	I_ADC_GAIN_16,		//I通道adc 16倍增益
	I_ADC_GAIN_4,		//I通道adc 4倍增益
	I_ADC_GAIN_1,		//I通道adc 1倍增益
}EN_GIA;

typedef enum{
	U_ADC_GAIN_4 = 0,	//U通道adc 4倍增益
	U_ADC_GAIN_1,		//U通道adc 1倍增益
}EN_GU;

typedef enum{
	ADCPDN_DISACTIVE = 0,	//不使能adc
	ADCPDN_ACTIVE,			//使能adc
}EN_ADCPDN;

//=============================================================
//V9240 系统控制寄存器  
//=============================================================
typedef struct{
	u32 Reserve_Bit0:1;
	EN_PGAU_ PGAU:1;			//配置电压（U）通道数字增益。0：×1；1：×4。
	EN_BPHPF BPHPF:1;			//旁路高通滤波器，默认信号处理中开启高通滤波器，要做直流计量时需要使能该位。
	EN_IESUL IESUL:1;			//使能电压符号位输出，默认禁止。0：禁止；1：使能。
	u32 Reserve_Bit4:1;
	u32 IEHSE:1;				//为保证计量芯片正常工作，必须写入默认值。
	EN_RCX12 RCX12:1;			//RC 基频调整。计量芯片设计的默认电压频率是 50Hz，当需要应用到 60Hz系统时，可以将计量芯片内部的高频 RC 时钟的默认频率 3.2768MHZ 提高到原来的 1.2 倍。
	u32 RCTRIM:5;				//调节内部高频 RC 时钟频率（理想值是 3.2768MHz）
	EN_SHORT SHORTI:1;			//当 I 通道输入直流信号时，可将该位置 1 将 I 通道放大器端短路，获得 ADC本身的偏置值。仅直流计量初始化时候需要打开用于获取 ADC 自身引入的直流偏置，正常计量时候关闭
	EN_SHORT SHORTU:1;			//当 U 通道输入直流信号时，可将该位置 1 将 U 通道放大器端短路，获得 ADC本身的偏置值。仅直流计量初始化时候需要打开用于获取ADC 自身引入的直流偏置，正常计量时候关闭
	u32 RESTL:2;				//电压基准（Bandgap）电路的温度系数粗调节，00：0ppm；01：+70ppm；10：-140ppm；11：-70ppm
	u32 REST:3;					//电压基准（Bandgap）电路的温度系数微调节，000：0ppm；001：+10ppm；010：+20ppm；011：+30ppm；100：-40ppm；101：-30ppm；110：-20ppm；111：-10ppm
	EN_CKMDIV CKMDIV:1;			//电能计量时钟频率，b0:3.2768MHz、b1:819.2kHz
	EN_ADCLKSEL ADCLKSEL:2;		//adc采样频率，b00:819.2kHz、b01:409.6kHz、b10:204.8kHz、b11:102.4kHz
	EN_GIA GIA:2;				//电流通道adc增益，b00:32倍增益、b01:16倍增益、b10:4倍增益、b11:1倍增益
	u32 Reserve_Bit24_25:2;
	EN_GU GU:1;					//电压通道adc增益，b0:4倍增益、b1:1倍增益
	EN_ADCPDN ADCIAPDN:1;		//电流通道adc使能位。0：禁止；1：使能。
	u32 Reserve_Bit28:1;
	EN_ADCPDN ADCUPDN:1;		//电压通道adc使能位。0：禁止；1：使能。
	u32 Reserve_Bit30_31:2;
}ST_V9240_SC_B;

typedef union{
	u32 Value;
	ST_V9240_SC_B Value_B;
}UN_V9240_SC;        //System control





typedef enum{
	CSEL_0_PTC = 0,		//不调整
	CSEL_33_PTC,		//增加 33%
	CSEL_66_PTC,		//增加 66%
	CSEL_100_PTC,		//增加 100%
}EN_CSEL;

//=============================================================
//V9240 模拟控制寄存器  
//=============================================================
typedef struct{
	u32 Reserve_Bit0_27:28;
	EN_CSEL CSEL:2;		//调整 I 通道 ADC 的密勒电容。00：不调整；01：增加 33%；10：增加 66%；11：增加 100%。
	u32 Reserve_Bit30_31:2;
}ST_V9240_AC_B;

typedef union{
	u32 Value;
	ST_V9240_AC_B Value_B;
}UN_V9240_AC;        //Analog control





//=============================================================
//V9240 复位寄存器参数值  
//=============================================================
typedef enum{
	V9240_SFTRST = 0x4572BEAF,
}EN_V9240_SFTRST;



#endif

