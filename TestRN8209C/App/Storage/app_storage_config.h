#ifndef _APP_STORAGE_CONFIG_H_
#define _APP_STORAGE_CONFIG_H_

#include "app_w25qxx.h"





#define W25QXX_SECTOR_INIT_FLAG 0xC5B0

#define PROGRAM_SECTORS 25		//25个sector

typedef enum{
	STORAGE_UPGRADE = 0,
	STORAGE_DEV_ADDR,
	STORAGE_SYS_CFG,
	STORAGE_SYS_DATA,
	STORAGE_HIS_RECORD,
	STORAGE_RN8209C,			//预留，需要增加配置的时候，将这些保留区占用一个
	STORAGE_V9240,
	STORAGE_RESERVE_1,
	STORAGE_RESERVE_2,
	STORAGE_RESERVE_3,
	STORAGE_RESERVE_4,
	STORAGE_RESERVE_5,
	STORAGE_RESERVE_6,
	STORAGE_RESERVE_7,
	STORAGE_RESERVE_8,
	STORAGE_RESERVE_9,
	STORAGE_SYS_PROGRAM,
	STORAGE_SYS_POWER = STORAGE_SYS_PROGRAM + PROGRAM_SECTORS,
	STORAGE_MAX
}EN_STORAGE_TYPE;


#endif

