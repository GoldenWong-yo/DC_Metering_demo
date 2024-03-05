#ifndef _APP_STORAGE_V9240_H_
#define _APP_STORAGE_V9240_H_

#include "app_storage_drv.h"
#include "V9240_reg.h"





typedef struct{
	ST_STORAGE_CHECK StorageCheck;
	ST_V9240_CP_REG CP_Reg;
	ST_MY_FLOAT E;
	ST_MY_FLOAT E1;
}ST_STORAGE_V9240;

extern ST_STORAGE_V9240 Storage_V9240;


void Init_Storage_V9240_Variables(void);
void Init_Storage_V9240_Info(ST_STORAGE_INFO *StorageInfo);
void Load_Storage_V9240(void);

#endif


