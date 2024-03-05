#ifndef _V9240_REG_H_
#define _V9240_REG_H_

#include "V9240_reg_addr.h"
#include "V9240_reg_cfg.h"





extern u32 V9240_CheckSum;

//=============================================================
//V9240 状态及控制寄存器  
//=============================================================
typedef struct{
	UN_V9240_SS SysSts;        //系统状态寄存器
	UN_V9240_SC SysCtrl;       //系统控制寄存器
	u32 AnaCtrl0;              //模拟控制寄存器0
	UN_V9240_AC AnaCtrl1;      //模拟控制寄存器1
	u32 SysStsClr;             //系统状态清除寄存器
	EN_V9240_SFTRST SFTRST;    //软件复位寄存器，软件复位寄存器写入0x4572BEAF，复位所有电路
}ST_V9240_SSC_REG;        //System Status and Control

//=============================================================
//V9240 计量数据寄存器  
//=============================================================
typedef struct{
	s32 FREQINST;      //瞬时频率值
	s32 PAINST;        //A路瞬时有功功率值
	s32 QINST;         //瞬时无功功率值
	s32 IAINST;        //A路瞬时电流有效值
	s32 UINST;         //瞬时电压有效值
	s32 PAAVG;         //A路平均有功功率值
	s32 QAVG;          //平均无功功率值 
	s32 FRQAVG;        //平均频率值 
	s32 IAAVG;         //A路平均电流有效值
	s32 UAVG;          //平均电压有效值
	s32 PBINST;        //B路瞬时有功功率值
	s32 IBINST;        //B路瞬时电流有效值
	s32 PBAVG;         //B路平均有功功率值
	s32 IBAVG;         //B路平均电流有效值
	s32 UDCINST;       //电压瞬时直流分量值
	s32 IADCINST;      //A路瞬时直流分量值
	s32 IBDCINST;      //B路瞬时直流分量值
	s32 ZXDATREG;      //电压过零点前一次采样值
	s32 ZXDAT;         //电压过零点当前点采样值
	s32 PHDAT;         //电压相位数据
	s32 T8BAUD;        //当前波特率通信时连续8bit下降沿时间数据
}ST_V9240_MD_REG;     //Measurement data

//=============================================================
//V9240 校表参数寄存器  
//=============================================================
typedef struct{
	s32 PAC;           //A通道有功功率比差校正
	s32 PHC;           //有功功率角差校正
	s32 PADCC;         //A通道有功功率小信号校正
	s32 QAC;           //A路无功功率比差校正
	s32 QBC;           //B路无功功率比差校正
	s32 QADCC;         //A路无功功率小信号校正
	s32 QBDCC;         //B路无功功率小信号校正
	s32 IAC;           //A路电流有效值比差校正
	s32 IADCC;         //A路电流有效值小信号校正
	s32 UC;            //电压有效值比差校正
	s32 PBC;           //B路有功功率比差校正
	s32 PBDCC;         //B路有功功率小信号校正
	s32 IBC;           //B路电流有效值比差校正
	s32 IBDCC;         //B路电流有效值小信号校正
	s32 IAADCC;        //A路电流ADC直流偏置校正
	s32 IBADCC;        //B路电流ADC直流偏置校正
	s32 UADCC;         //电压ADC直流偏置校正
	s32 BPFPARA;       //带通滤波器系数
	s32 UDCC;          //电压有效值小信号校正
	s32 CKSUM;         //校验和寄存器
}ST_V9240_CP_REG;     //Calibration parameters

typedef struct{
	ST_V9240_SSC_REG SSC;     //状态及控制寄存器
	ST_V9240_MD_REG MD;       //计量数据寄存器
	ST_V9240_CP_REG CP;       //校表参数寄存器
}ST_V9240_REG;

extern ST_V9240_REG V9240_Reg;


bool If_Found_V9240_Reg(EN_V9240_REG_ADDR RegAddr);
u32 V9240_Reg_Value_2_Buf(EN_V9240_REG_ADDR RegAddr, u8 Data[]);
EN_Global_Status V9240_Parse_Cmd(EN_V9240_REG_ADDR RegAddr, u8 Data[], u8 Len);


#endif

