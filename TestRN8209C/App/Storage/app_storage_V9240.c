#include "app_storage_V9240.h"
#include "app_storage.h"
#include "app_test.h"
#include "V9240_init.h"





ST_STORAGE_V9240 Storage_V9240;


void Init_Storage_V9240_Variables(void)
{
	memset(&Storage_V9240, 0, sizeof(Storage_V9240));
}

static void Init_Storage_V9240_Data(void)
{
	Init_Storage_V9240_Variables();

//	Storage_Set_NeedSave_Flag(STORAGE_V9240);
}

void Init_Storage_V9240_Info(ST_STORAGE_INFO *StorageInfo)
{
	StorageInfo->Type     = STORAGE_V9240;
	StorageInfo->InitData = Init_Storage_V9240_Data;
	StorageInfo->RamAddr  = (ST_STORAGE_DATA_P *)&Storage_V9240;
	StorageInfo->Len      = sizeof(Storage_V9240);
}

static void Check_Storage_V9240_Value(void)
{
	bool NeedSave = false;

	if(NeedSave)
	{
		Storage_Set_NeedSave_Flag(STORAGE_V9240);
	}
}

void Load_Storage_V9240(void)
{
	ST_STORAGE_INFO StorageInfo;
	
	Init_Storage_V9240_Info(&StorageInfo);
	Storage_Check_Main_Area(StorageInfo);			//读取主存储区和备份存储区的数据，检查存储的数据是否正确
	
	Check_Storage_V9240_Value();					//对一些可能造成参数越界，系统死机的变量，检查参数的有效性
}



