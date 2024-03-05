#include "app_w25qxx.h"
#include "spi.h"
#include "gpio.h"
#include <string.h>





//指令表
#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg1		0x05
#define W25X_ReadStatusReg2		0x35
#define W25X_ReadStatusReg3		0x15
#define W25X_WriteStatusReg1	0x01
#define W25X_WriteStatusReg2	0x31
#define W25X_WriteStatusReg3	0x11
#define W25X_ReadData			0x03
#define W25X_FastReadData		0x0B
#define W25X_FastReadDual		0x3B
#define W25X_PageProgram		0x02
#define W25X_BlockErase			0xD8
#define W25X_SectorErase		0x20
#define W25X_ChipErase			0xC7
#define W25X_PowerDown			0xB9
#define W25X_ReleasePowerDown	0xAB
#define W25X_DeviceID			0xAB
#define W25X_ManufactDeviceID	0x90
#define W25X_JedecDeviceID		0x9F
#define W25X_Enable4ByteAddr	0xB7
#define W25X_Exit4ByteAddr		0xE9

#define SPI_CS_ENABLE  HAL_GPIO_WritePin(W25Q_CS_GPIO_Port, W25Q_CS_Pin, GPIO_PIN_RESET)
#define SPI_CS_DISABLE HAL_GPIO_WritePin(W25Q_CS_GPIO_Port, W25Q_CS_Pin, GPIO_PIN_SET)

typedef enum{
	W25QXX_IDLE = 0,
	W25QXX_BUSY
}EN_W25QXX_STATE;

ST_W25QXX_INFO W25qxxInfo;


static void W25qxx_Init_Variables(void)
{
	memset(&W25qxxInfo, 0, sizeof(W25qxxInfo));

#ifdef W25QXX_END_ADDR
	W25qxxInfo.SectorNum = W25QXX_END_ADDR / W25QXX_SECTOR_SIZE;
	W25qxxInfo.HalfSectorIdx = W25qxxInfo.SectorNum / 2;
#else
	W25qxxInfo.SectorNum = 256;
	W25qxxInfo.HalfSectorIdx = 128;
#endif
}

static u8 W25qxx_ReadWrite_Byte(u8 TxData)
{
	u8 RxData = 0xff;
	
	HAL_SPI_TransmitReceive(&hspi2, &TxData, &RxData, 1, 1000);
	
	return RxData;
}

static void W25qxx_Send_Cmd(u8 Cmd)
{
	W25qxx_ReadWrite_Byte(Cmd);
}

static void W25qxx_Send_Addr(u32 Addr)
{
	if(W25QXX_TYPE == W25Q256)						//如果是W25Q256的话地址为4字节的，要发送最高8位
	{
		W25qxx_ReadWrite_Byte((u8)(Addr >> 24));
	}
	W25qxx_ReadWrite_Byte((u8)(Addr >> 16));		//发送24bit地址
	W25qxx_ReadWrite_Byte((u8)(Addr >> 8));
	W25qxx_ReadWrite_Byte((u8)Addr);
}

static void W25qxx_Send_Data(u8 *Data,u32 Len)
{
	u32 Idx = 0;
	
	for(Idx=0; Idx<Len; Idx++)
	{
		W25qxx_ReadWrite_Byte(Data[Idx]);
	}
}

static void W25qxx_Get_Data(u8 *Data,u32 Len)
{
	u32 Idx = 0;
	
	for(Idx=0; Idx<Len; Idx++)
	{
		Data[Idx] = W25qxx_ReadWrite_Byte(0xff);
	}
}

static void W25qxx_Write_Enable(void)
{
	SPI_CS_ENABLE;
	W25qxx_Send_Cmd(W25X_WriteEnable);
	SPI_CS_DISABLE;
}

static void W25qxx_Write_Disable(void)
{
	SPI_CS_ENABLE;
	W25qxx_Send_Cmd(W25X_WriteDisable);
	SPI_CS_DISABLE;
}

static u8 W25qxx_Read_Reg(u8 Reg)
{
	u8 RegValue = 0;
	
	SPI_CS_ENABLE;
	W25qxx_Send_Cmd(Reg);
	W25qxx_Get_Data(&RegValue,1);
	SPI_CS_DISABLE;
	
	return RegValue;
}

static EN_W25QXX_STATE W25qxx_Check_Busy(void)
{
	u8 RegValue;
	
	RegValue = W25qxx_Read_Reg(W25X_ReadStatusReg1);
	
	if((RegValue & 0x01) == 0x01)
	{
		return W25QXX_BUSY;
	}else{
		return W25QXX_IDLE;
	}
}

static void W25qxx_Wait_Busy(void)
{
	u32 WaitTick = 0;
	
	Reset_Tick(&WaitTick);
	
	while(1)
	{
		if(W25qxxInfo.ErrorFlag)
		{
			break;
		}
		else if(W25qxx_Check_Busy() == W25QXX_IDLE)
		{
			break;
		}
		else if(Tick_Timeout(&WaitTick, TIME_300MS))
		{
			W25qxxInfo.ErrorFlag = true;
			break;
		}
	}
	//HAL_Delay(10);
}

//初始化SPI FLASH的IO口
void W25qxx_Init(void)
{
	u8 Temp;

	W25qxx_Init_Variables();

	W25qxx_Wait_Busy();

	//W25QXX_TYPE = W25QXX_ReadID();					//读取FLASH ID.
	if(W25QXX_TYPE == W25Q256)							//SPI FLASH为W25Q256
	{
		Temp = W25qxx_Read_Reg(W25X_ReadStatusReg3);	//读取状态寄存器3，判断地址模式
		if((Temp & 0x01) == 0)							//如果不是4字节地址模式,则进入4字节地址模式
		{
			SPI_CS_ENABLE;								//选中
			W25qxx_Send_Cmd(W25X_Enable4ByteAddr);		//发送进入4字节地址模式指令
			SPI_CS_DISABLE;								//取消片选
		}
	}
}

void W25qxx_Erase_Chip(void)
{
	W25qxx_Wait_Busy();
	
	W25qxx_Write_Enable();
	
	SPI_CS_ENABLE;
	W25qxx_Send_Cmd(W25X_ChipErase);
	SPI_CS_DISABLE;
	
	//W25qxx_Write_Disable();
	
	//W25qxx_Wait_Busy();
}

void W25qxx_Erase_Sector(u32 Addr)
{
	if(Addr % W25QXX_SECTOR_SIZE != 0)
	{
		Addr = (Addr / W25QXX_SECTOR_SIZE) * W25QXX_SECTOR_SIZE;
	}
	
	W25qxx_Wait_Busy();
	
	W25qxx_Write_Enable();
	
	SPI_CS_ENABLE;                         //使能器件
	W25qxx_Send_Cmd(W25X_SectorErase);
	W25qxx_Send_Addr(Addr);
	SPI_CS_DISABLE;
	
	//W25qxx_Write_Disable();
	
	//W25qxx_Wait_Busy();
}

static bool W25qxx_Write_Page(u32 Addr,u8 *Data,u32 Len)
{
	u32 PageOffset = 0;
	u32 PageRemain = 0;
	
	if(Len > W25QXX_PAGE_SIZE){
		return false;
	}
	
	PageOffset = Addr % W25QXX_PAGE_SIZE;
	PageRemain = W25QXX_PAGE_SIZE - PageOffset;
	if(Len > PageRemain)
	{
		return false;
	}
	
	W25qxx_Wait_Busy();
	
	W25qxx_Write_Enable();
	
	SPI_CS_ENABLE;
	W25qxx_Send_Cmd(W25X_PageProgram);
	W25qxx_Send_Addr(Addr);
	W25qxx_Send_Data(Data,Len);
	SPI_CS_DISABLE;
	
	//W25qxx_Write_Disable();
	
	//W25qxx_Wait_Busy();
	
	return true;
}

static bool W25qxx_Write_Sector(u32 Addr,u8 *Data,u32 Len)
{
	u32 SectorOffset = 0;
	u32 SectorRemain = 0;
	u32 PageOffset = 0;
	u32 PageRemain = 0;
	
	if(Len > W25QXX_SECTOR_SIZE)
	{
		return false;
	}
	
	SectorOffset = Addr % W25QXX_SECTOR_SIZE;
	SectorRemain = W25QXX_SECTOR_SIZE - SectorOffset;
	if(Len > SectorRemain)
	{
		return false;
	}
	
	PageOffset = Addr % W25QXX_PAGE_SIZE;
	PageRemain = W25QXX_PAGE_SIZE - PageOffset;
	if(PageRemain >= Len)
	{
		PageRemain = Len;
	}
	
	while(1)
	{
		if(W25qxx_Write_Page(Addr,Data,PageRemain))
		{
			if(PageRemain == Len)
			{
				return true;
			}else{
				Addr += PageRemain;
				Data += PageRemain;
				Len  -= PageRemain;
				if(Len > W25QXX_PAGE_SIZE)
				{
					PageRemain = W25QXX_PAGE_SIZE;
				}else{
					PageRemain = Len;
				}
			}
		}else{
			return false;
		}
	}
}

bool W25qxx_Write(u32 Addr,u8 *Data,u32 Len)
{
	u32 TempIdx;
	u32 SectorPosition;
	u32 SectorOffset;
	u32 SectorRemain;
	u8 SectorBuf[W25QXX_SECTOR_SIZE];
	
	if(W25qxxInfo.ErrorFlag)
	{
		return false;
	}
	
	SectorPosition = Addr / W25QXX_SECTOR_SIZE;					//扇区地址
	SectorOffset   = Addr % W25QXX_SECTOR_SIZE;					//在扇区内的偏移
	SectorRemain   = W25QXX_SECTOR_SIZE - SectorOffset;			//扇区剩余空间大小
	//printf("ad:%X,nb:%X\r\n",Addr,Len);//测试用
	
	if(SectorRemain >= Len)
	{
		SectorRemain = Len;										//不大于4096个字节
	}
	
	while(1)
	{
		memset(SectorBuf,0xff,W25QXX_SECTOR_SIZE);
		
		W25qxx_Read(SectorPosition*W25QXX_SECTOR_SIZE,SectorBuf,W25QXX_SECTOR_SIZE);	//读出整个扇区的内容
		
		for(TempIdx=0; TempIdx<SectorRemain; TempIdx++)			//校验数据
		{
			if(SectorBuf[SectorOffset+TempIdx] != 0xff)
			{
				break;											//需要擦除
			}
		}
		
		if(TempIdx < SectorRemain)								//需要擦除
		{
			W25qxx_Erase_Sector(Addr);							//擦除这个扇区
			
			for(TempIdx=0; TempIdx<SectorRemain; TempIdx++)		//复制
			{
				SectorBuf[TempIdx+SectorOffset] = Data[TempIdx];
			}
			
			W25qxx_Write_Sector(SectorPosition*W25QXX_SECTOR_SIZE,SectorBuf,W25QXX_SECTOR_SIZE);	//写入整个扇区
		}else{
			W25qxx_Write_Sector(Addr,Data,SectorRemain);		//写已经擦除了的,直接写入扇区剩余区间.
		}
		
		if(Len == SectorRemain)
		{
			return true;				//写入结束了
		}else{							//写入未结束
			SectorPosition++;			//扇区地址增1
			SectorOffset = 0;			//偏移位置为0

			Data += SectorRemain;		//指针偏移
			Addr += SectorRemain;		//写地址偏移
			Len  -= SectorRemain;		//字节数递减

			if(Len > W25QXX_SECTOR_SIZE)
			{
				SectorRemain = W25QXX_SECTOR_SIZE;		//下一个扇区还是写不完
			}else{
				SectorRemain = Len;		//下一个扇区可以写完了
			}
		}
	}
}

void W25qxx_Read(u32 Addr,u8 *Data,u32 Len)
{
	W25qxx_Wait_Busy();
	
	SPI_CS_ENABLE;
	W25qxx_Send_Cmd(W25X_ReadData);
	W25qxx_Send_Addr(Addr);
	W25qxx_Get_Data(Data,Len);
	SPI_CS_DISABLE;
}

void W25qxx_Test(void)
{
	u8 TxData = 0;
	u8 RxData = 0;
	
	while(1)
	{
		TxData = 5;
		W25qxx_Write(0,&TxData,1);
		W25qxx_Read(0,&RxData,1);
		HAL_Delay(3000);
		
		TxData = 35;
		W25qxx_Write(0,&TxData,1);
		W25qxx_Read(0,&RxData,1);
		HAL_Delay(3000);
		
//		W25qxx_Erase_Sector(0);
//		W25QXX_Erase_Sector(0);
//		W25qxx_Read(0,&RxData,1);
//		HAL_Delay(3000);
	}
}

bool Get_W25qxx_Status(void)
{
	return W25qxxInfo.ErrorFlag;
}



