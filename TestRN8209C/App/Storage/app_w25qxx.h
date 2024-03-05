#ifndef _APP_W25QXX_H_
#define _APP_W25QXX_H_

#include "app_global.h"



typedef enum{
	W25Q80 = 0,
	W25Q16,
	W25Q32,
	W25Q64,
	W25Q128,
	W25Q256
}EN_W25QXX_TYPE;

#define W25QXX_TYPE W25Q80

#define W25QXX_START_ADDR 0x000000

#if (W25QXX_TYPE == W25Q80)
	#define W25QXX_END_ADDR  0x100000
#elif (W25QXX_TYPE == W25Q32)
	#define W25QXX_END_ADDR 0x400000
#endif

#define W25QXX_PAGE_SIZE 256
#define W25QXX_SECTOR_SIZE 4096

typedef struct{
	u32 SectorNum;
	u32 HalfSectorIdx;
	bool ErrorFlag;
}ST_W25QXX_INFO;

extern ST_W25QXX_INFO W25qxxInfo;


void W25qxx_Init(void);
void W25qxx_Erase_Chip(void);
void W25qxx_Erase_Sector(u32 Addr);
bool W25qxx_Write(u32 Addr,u8 *Data,u32 Len);
void W25qxx_Read(u32 Addr,u8 *Data,u32 Len);
void W25qxx_Test(void);
bool Get_W25qxx_Status(void);

#endif // !_APP_W25QXX_DRV_H_
