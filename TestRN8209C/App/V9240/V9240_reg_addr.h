#ifndef _V9240_REG_ADDR_H_
#define _V9240_REG_ADDR_H_





//****************************************************************
//V9240 寄存器定义 
//****************************************************************
typedef enum{

	//=============================================================
	//V9240 状态及控制寄存器  
	//=============================================================
	Addr_SysSts    = 0x00CA,     //系统状态寄存器
	Addr_SysCtrl   = 0x0180,     //系统控制寄存器
	Addr_AnaCtrl0  = 0x0182,     //模拟控制寄存器0
	Addr_AnaCtrl1  = 0x0183,     //模拟控制寄存器1
	Addr_SysStsClr = 0x019D,     //系统状态清除寄存器
	Addr_SFTRST    = 0x01BF,     //软件复位寄存器，软件复位寄存器    写入0x4572BEAF，复位所有电路

	//=============================================================
	//V9240 计量数据寄存器  
	//=============================================================
	Addr_FREQINST  = 0x00CB,     //瞬时频率值
	Addr_PAINST    = 0x00CC,     //A路瞬时有功功率值
	Addr_QINST     = 0x00CD,     //瞬时无功功率值
	Addr_IAINST    = 0x00CE,     //A路瞬时电流有效值
	Addr_UINST     = 0x00CF,     //瞬时电压有效值
	Addr_PAAVG     = 0x00D0,     //A路平均有功功率值
	Addr_QAVG      = 0x00D1,     //平均无功功率值
	Addr_FRQAVG    = 0x00D2,     //平均频率值
	Addr_IAAVG     = 0x00D3,     //A路平均电流有效值
	Addr_UAVG      = 0x00D4,     //平均电压有效值
	Addr_PBINST    = 0x00D5,     //B路瞬时有功功率值
	Addr_IBINST    = 0x00D6,     //B路瞬时电流有效值
	Addr_PBAVG     = 0x00D7,     //B路平均有功功率值
	Addr_IBAVG     = 0x00D8,     //B路平均电流有效值
	Addr_UDCINST   = 0x00D9,     //电压瞬时直流分量值
	Addr_IADCINST  = 0x00DA,     //A路瞬时直流分量值
	Addr_IBDCINST  = 0x00DB,     //B路瞬时直流分量值
	Addr_ZXDATREG  = 0x00DC,     //电压过零点前一次采样值
	Addr_ZXDAT     = 0x00DD,     //电压过零点当前点采样值
	Addr_PHDAT     = 0x00DE,     //电压相位数据
	Addr_T8BAUD    = 0x00E0,     //当前波特率通信时连续8bit下降沿时间数据
//	Addr_T1BAUD    = 0x00E0,     //当前波特率通信时连续1bit下降沿时间数据

	//=============================================================
	//V9240 校表参数寄存器  
	//=============================================================
	Addr_PAC       = 0x00F6,     //A通道有功功率比差校正
	Addr_PHC       = 0x00F7,     //有功功率角差校正
	Addr_PADCC     = 0x00F8,     //A通道有功功率小信号校正
	Addr_QAC       = 0x00F9,     //A路无功功率比差校正
	Addr_QBC       = 0x00FA,     //B路无功功率比差校正
	Addr_QADCC     = 0x00FB,     //A路无功功率小信号校正
	Addr_QBDCC     = 0x00FC,     //B路无功功率小信号校正
	Addr_IAC       = 0x00FD,     //A路电流有效值比差校正
	Addr_IADCC     = 0x00FE,     //A路电流有效值小信号校正
	Addr_UC        = 0x00FF,     //电压有效值比差校正
	Addr_PBC       = 0x0100,     //B路有功功率比差校正
	Addr_PBDCC     = 0x0101,     //B路有功功率小信号校正
	Addr_IBC       = 0x0102,     //B路电流有效值比差校正
	Addr_IBDCC     = 0x0103,     //B路电流有效值小信号校正
	Addr_IAADCC    = 0x0104,     //A路电流ADC直流偏置校正
	Addr_IBADCC    = 0x0105,     //B路电流ADC直流偏置校正
	Addr_UADCC     = 0x0106,     //电压ADC直流偏置校正
	Addr_BPFPARA   = 0x0107,     //带通滤波器系数
	Addr_UDCC      = 0x0108,     //电压有效值小信号校正
	Addr_CKSUM     = 0x0109,     //校验和寄存器

}EN_V9240_REG_ADDR;



#endif

