#include "app_storage_RN8209C.h"
#include "app_storage.h"
#include "app_test.h"
#include "RN8209C_init.h"





ST_STORAGE_RN8209C Storage_RN8209C;


void Init_Storage_RN8209C_Variables(void)
{
	memset(&Storage_RN8209C, 0, sizeof(Storage_RN8209C));
}

static void Init_Storage_RN8209C_Data(void)
{
	Init_Storage_RN8209C_Variables();

//	Storage_Set_NeedSave_Flag(STORAGE_RN8209C);
}

void Init_Storage_RN8209C_Info(ST_STORAGE_INFO *StorageInfo)
{
	StorageInfo->Type     = STORAGE_RN8209C;
	StorageInfo->InitData = Init_Storage_RN8209C_Data;
	StorageInfo->RamAddr  = (ST_STORAGE_DATA_P *)&Storage_RN8209C;
	StorageInfo->Len      = sizeof(Storage_RN8209C);
}

static void Check_Storage_RN8209C_Value(void)
{
	bool NeedSave = false;

	if(NeedSave)
	{
		Storage_Set_NeedSave_Flag(STORAGE_RN8209C);
	}
}

void Load_Storage_RN8209C(void)
{
	ST_STORAGE_INFO StorageInfo;
	
	Init_Storage_RN8209C_Info(&StorageInfo);
	Storage_Check_Main_Area(StorageInfo);			//读取主存储区和备份存储区的数据，检查存储的数据是否正确
	
	Check_Storage_RN8209C_Value();					//对一些可能造成参数越界，系统死机的变量，检查参数的有效性
}



