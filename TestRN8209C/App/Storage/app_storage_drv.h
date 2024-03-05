#ifndef _APP_STORAGE_DRV_H_
#define _APP_STORAGE_DRV_H_

#include "app_storage_config.h"
#include "app_tool.h"





typedef enum{
	WRITE_MAIN_FIRST = 0,			//先写主存储区
	WRITE_BACKUP_FIRST,				//先写备份存储区
	WRITE_MAIN_ONLY,				//只写主存储区
	WRITE_BACKUP_ONLY				//只写备份存储区
}EN_WRITE_FLASH_ORDER;

typedef struct{
	u16 InitFlag;					//当前flash块的初始化标志
	u16 FlashLen;					//当前flash块里面的有效数据长度
	u32 SectorWriteTimes;			//当前flash块被擦写的次数
	u16 CheckSum;					//当前flash块的数据校验标志
}ST_STORAGE_CHECK;

typedef struct{
	ST_STORAGE_CHECK StorageCheck;
	u8 *DataInfo;
}ST_STORAGE_DATA_P;

typedef struct{
	EN_STORAGE_TYPE Type;
	void (* InitData)(void);
	ST_STORAGE_DATA_P *RamAddr;
	u16 Len;
}ST_STORAGE_INFO;


void Storage_Write_Data_To_Flash(ST_STORAGE_INFO StorageInfo, EN_WRITE_FLASH_ORDER WriteOrder);
void Storage_Check_Main_Area(ST_STORAGE_INFO StorageInfo);


#endif

