#ifndef _RN8209C_REG_ADDR_H_
#define _RN8209C_REG_ADDR_H_

#include "app_global.h"





//---- RN8209C 地址定义----//

typedef enum{
	ADDR_SYSCON = 0x00,		//系统控制寄存器
	ADDR_EMUCON = 0x01,		//计量控制寄存器
	ADDR_HFConst = 0x02,	//脉冲频率寄存器
	ADDR_PStart = 0x03,		//有功起动功率设置
	ADDR_DStart = 0x04,		//自定义电能起动功率设置
	ADDR_GPQA = 0x05,
	ADDR_GPQB = 0x06,
	ADDR_PhsA = 0x07,
	ADDR_PhsB = 0x08,
	ADDR_QPHSCAL = 0x09,
	ADDR_APOSA = 0x0a,		//通道A有功功率Offset校正寄存器
	ADDR_APOSB = 0x0b,		//通道B有功功率Offset校正寄存器
	ADDR_RPOSA = 0x0c,
	ADDR_RPOSB = 0x0d,
	ADDR_IARMSOS = 0x0e,
	ADDR_IBRMSOS = 0x0f,
	ADDR_IBGain = 0x10,		//电流通道B增益设置
	ADDR_D2FPL = 0x11,		//自定义功率寄存器低
	ADDR_D2FPH = 0x12,		//自定义功率寄存器高
	ADDR_DCIAH = 0x13,
	ADDR_DCIBH = 0x14,
	ADDR_DCUH = 0x15,
	ADDR_DCL = 0x16,
	ADDR_EMUCON2 = 0x17,	//计量控制寄存器2

	ADDR_PFCnt = 0x20,		//通道A快速有功脉冲计数
	ADDR_DFcnt = 0x21,		
	ADDR_IARMS = 0x22,		//通道A电流有效值
	ADDR_IBRMS = 0x23,		//通道B电流有效值
	ADDR_URMS = 0x24,		//电压有效值
	ADDR_UFreq = 0x25,		//频率
	ADDR_PowerPA = 0x26,	//有功功率A
	ADDR_PowerPB = 0x27,	//有功功率B
	ADDR_PowerQ = 0x28,		//无功功率
	ADDR_EnergyP = 0x29,	//有功能量，默认通道A
	ADDR_EnergyP2 = 0x2a,
	ADDR_EnergyD = 0x2b,	//有功能量，配置成通道B
	ADDR_EnergyD2 = 0x2c,
	ADDR_EMUStatus = 0x2d,	//计量状态寄存器
	ADDR_SPL_IA = 0x30,		//IA通道电流采样值
	ADDR_SPL_IB = 0x31,		//IB通道电流采样值
	ADDR_SPL_U = 0x32,		//U通道电流采样值
	ADDR_UFreq2 = 0x35,

	ADDR_IE = 0x40,
	ADDR_IF = 0x41,
	ADDR_RIF = 0x42,

	ADDR_SysStatus = 0x43,
	ADDR_RData = 0x44,
	ADDR_WData = 0x45,
	ADDR_DeviceID = 0x7f,

	ADDR_SpecialFunc = 0xea,		//特殊命令（抽象为一个寄存器）
}EN_RN8209C_REG_ADDR;


#endif

