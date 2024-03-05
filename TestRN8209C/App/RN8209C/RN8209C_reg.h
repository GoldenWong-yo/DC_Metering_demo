#ifndef _RN8209C_REG_H_
#define _RN8209C_REG_H_

#include "RN8209C_reg_addr.h"
#include "RN8209C_reg_cfg.h"





extern u16 RN8209C_CheckSum;

//---- RN8209C 寄存器缓存----//

typedef struct{
	UN_RN8209C_SC SYSCON;
	UN_RN8209C_EMC EMUCON;
	u16 HFConst;
	u16 PStart;
	u16 DStart;
	s16 GPQA;
	s16 GPQB;
	s8 PhsA;
	s8 PhsB;
	s16 QPHSCAL;
	s16 APOSA;
	s16 APOSB;
	s16 RPOSA;
	s16 RPOSB;
	s16 IARMSOS;
	s16 IBRMSOS;
	s16 IBGain;
	u16 D2FPL;
	s16 D2FPH;
	u16 DCIAH;
	u16 DCIBH;
	u16 DCUH;
	u16 DCL;
	UN_RN8209C_EMC2 EMUCON2;
}ST_RN8209C_CTRL_REG;

typedef struct{
	u16 PFCnt;
	u16 DFcnt;
	s24 IARMS;
	s24 IBRMS;
	s24 URMS;
	u16 UFreq;
	s32 PowerPA;
	s32 PowerPB;
	s32 PowerQ;
	u24 EnergyP;
	u24 EnergyP2;
	u24 EnergyD;
	u24 EnergyD2;
	u24 EMUStatus;
	u24 SPL_IA;
	u24 SPL_IB;
	u24 SPL_U;
	u24 UFreq2;
}ST_RN8209C_DATA_REG;

typedef struct{
	u8 IE;
	u8 IF;
	u8 RIF;
}ST_RN8209C_INT_REG;

typedef struct{
	u8 SysStatus;
	u32 RData;
	u16 WData;
	u24 DeviceID;
	EN_RN8209C_SF SpecialFunc;
}ST_RN8209C_SSC_REG;//System Status and Control

typedef struct{
	ST_RN8209C_CTRL_REG Ctrl;	//控制寄存器
	ST_RN8209C_DATA_REG Data;	//数据和状态
	ST_RN8209C_INT_REG Int;		//中断
	ST_RN8209C_SSC_REG SSC;		//系统状态和控制
}ST_RN8209C_REG;

extern ST_RN8209C_REG RN8209C_Reg;


bool If_Found_RN8209C_Reg(EN_RN8209C_REG_ADDR RegAddr);
u8 RN8209C_Reg_Value_2_Buf(EN_RN8209C_REG_ADDR RegAddr, u8 Data[]);
EN_Global_Status RN8209C_Parse_Cmd(EN_RN8209C_REG_ADDR RegAddr, u8 Data[], u8 Len);


#endif

