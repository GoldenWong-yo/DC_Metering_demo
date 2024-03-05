#ifndef _RN8209C_REG_CFG_H_
#define _RN8209C_REG_CFG_H_

#include "app_global.h"





#define u24 u32
#define s24 s32

//********************************************************
//UART 波特率选择，只读，其值由硬件管脚 B1 和 B0 决定
//{B1,B0}=00, 读出值为 UartBr=7’h2E, 2400 波特率
//{B1,B0}=01, 读出值为 UartBr=7’h16, 4800 波特率
//{B1,B0}=10, 读出值为 UartBr=7’h0B, 9600 波特率
//{B1,B0}=11, 读出值为 UartBr=7’h05, 19200 波特率
//只在通信口选择为 UART 时有意义，在选择为 SPI 时读出为 0。
//请注意：uartbr[6:0]参与校验和计算，在通信口选择为 uart 时会影响到校验和计算结果。
//RN8209C 固定为 4800 波特率
//********************************************************
typedef enum{
	IA_GAIN_1 = 0,		//1倍增益
	IA_GAIN_2,			//2倍增益
	IA_GAIN_8,			//8倍增益
	IA_GAIN_16,			//16倍增益
}EN_PGAIA;

typedef enum{
	U_GAIN_1 = 0,
	U_GAIN_2,
	U_GAIN_4,
}EN_PGAU;

typedef enum{
	IB_GAIN_1 = 0,		//1倍增益
	IB_GAIN_2,			//2倍增益
	IB_GAIN_4,			//4倍增益
}EN_PGAIB;

typedef enum{
	ADC2ON_DISACTIVE = 0,	//不使能adc
	ADC2ON_ACTIVE,			//使能adc
}EN_ADC2ON;
	
typedef enum{
	BAUDRATE_2400 = 0x2e,
	BAUDRATE_4800 = 0x16,
	BAUDRATE_9600 = 0x0b,
	BAUDRATE_19200 = 0x05,
}EN_BAUDRATE;

typedef struct{
	EN_PGAIA PGAIA:2;
	EN_PGAU PGAU:2;
	EN_PGAIB PGAIB:2;
	EN_ADC2ON ADC2ON:1;			// =1：表示 ADC 电流通道 B 开启；=0：表示 ADC 电流通道 B 关闭，ADC 输出恒为 0
	u16 Reserve_Bit7:1;
	EN_BAUDRATE UartBr:7;		//UART 波特率选择，只读，其值由硬件管脚 B1 和 B0 决定
	u16 Reserve_Bit15:1;
}ST_RN8209C_SC_B;//System Control

typedef union{
	u16 Value;
	ST_RN8209C_SC_B Value_B;
}UN_RN8209C_SC;





typedef enum{
	Disactive_RUN = 0,		//关闭脉冲输出和自定义电能寄存器累加
	Active_RUN,				//使能脉冲输出和自定义电能寄存器累加
}EN_RUN;

typedef enum{
	Active_DHPF = 0,		//使能数字高通滤波器
	Disactive_DHPF, 		//关闭数字高通滤波器
}EN_DHPF_STA;

typedef enum{
	MOD_1 = 0,		//Qm=DataQ，正反向功率都参与累加，负功率有 REVQ 符号指示
	MOD_2,			//只累加正向功率
	MOD_3,			//Qm=|DataQ|，正反向功率都参与累加，无负功率符号指示
}EN_MOD;

typedef enum{
	Accumulating = 0,			//电能寄存器为累加型
	Clearing_After_Reading, 	//电能寄存器为读后清零型
}EN_EnergyCLR;

typedef struct{
	EN_RUN PRUN:1;			// PF 脉冲
	EN_RUN DRUN:1;			// QF 脉冲
	u16 CFSU:2;
	u16 CFSUEN:1;
	EN_DHPF_STA HPFUOFF:1;
	EN_DHPF_STA HPFIAOFF:1;
	u16 ZXCFG:1;
	u16 ZXD0:1;
	u16 ZXD1:1;
	EN_MOD PMOD:2;
	EN_MOD QMOD:2;
	EN_DHPF_STA HPFIBOFF:1;
	EN_EnergyCLR EnergyCLR:1;
}ST_RN8209C_EMC_B;//Energy Measure Control

typedef union{
	u16 Value;
	ST_RN8209C_EMC_B Value_B;
}UN_RN8209C_EMC;





typedef enum{
	D2FM_1 = 0,		//自定义电能输入选择为无功功率
	D2FM_2,			//自定义电能输入选择为通道 A 和通道 B 有功功率的矢量和
	D2FM_3,			//自定义电能输入选择为自定义功率寄存器 D2FP
	D2FM_4,			//自定义电能输入选择为通道 B 有功功率
}EN_D2FM;

typedef enum{
	UPMODE_3_495Hz = 0, 	//功率及有效值寄存器更新速度为 3.495Hz
	UPMODE_13_982Hz,		//功率及有效值寄存器更新速度为 13.982Hz
}EN_UPMODE;

typedef struct{
	u16 Reserve_Bit0_2:3;
	u16 Energy_fz:1;
	EN_D2FM D2FM:2;
	u16 ZXMODE:1;
	EN_UPMODE UPMODE:1;
	u16 PhsA0:1;
	u16 PhsB0:1;
	u16 Reserve_Bit10_11:2;
	u16 FreqCnt:2;
	u16 Reserve_Bit14_15:2;
}ST_RN8209C_EMC2_B;//Energy Measure Control 2

typedef union{
	u16 Value;
	ST_RN8209C_EMC2_B Value_B;
}UN_RN8209C_EMC2;





typedef enum{
	SF_UNLOCK = 0xe5,
	SF_LOCK = 0xdc,
	SF_SLC_CH_A = 0x5a,
	SF_SLC_CH_B = 0xa5,
	SF_RESET = 0xfa,
}EN_RN8209C_SF;//Special Function

#define IF_RN8209C_SF_OK(SpecialFunc) if(!(SpecialFunc == SF_UNLOCK \
										|| SpecialFunc == SF_LOCK \
										|| SpecialFunc == SF_SLC_CH_A \
										|| SpecialFunc == SF_SLC_CH_B \
										|| SpecialFunc == SF_RESET)) return


#endif

