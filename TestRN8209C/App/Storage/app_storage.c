#include "app_storage.h"
#include "app_storage_RN8209C.h"
#include "app_storage_V9240.h"





typedef struct{
	bool NeedSave;		//存储
	bool NeedClear;		//清除
}ST_STORAGE_ACT;

typedef struct{
	ST_STORAGE_ACT Act[STORAGE_MAX];
}ST_STORAGE;

static ST_STORAGE Storage;


void Init_Storage_Variables(void)
{
	memset(&Storage, 0, sizeof(Storage));

	Init_Storage_RN8209C_Variables();
}

void Storage_Init(void)
{
	W25qxx_Init();

	Load_Storage_RN8209C();
	Load_Storage_V9240();
}

static bool Init_Storage_Info(EN_STORAGE_TYPE Type, ST_STORAGE_INFO *StorageInfo)
{
	bool FoundStorage = false;

	switch(Type)
	{
		case STORAGE_RN8209C:
			Init_Storage_RN8209C_Info(StorageInfo);
			FoundStorage = true;
			break;
		case STORAGE_V9240:
			Init_Storage_V9240_Info(StorageInfo);
			FoundStorage = true;
			break;
		default:
			break;
	}

	return FoundStorage;
}





//存储操作
void Storage_Set_NeedSave_Flag(EN_STORAGE_TYPE Type)
{
	if(Type >= STORAGE_MAX)
	{
		return;
	}

	Storage.Act[Type].NeedSave = true;
}

EN_STORAGE_TYPE Check_Storage_Save_Task(void)
{
	EN_STORAGE_TYPE Type = STORAGE_MAX;

	for(Type=STORAGE_UPGRADE; Type<STORAGE_MAX; Type++)
	{
		if(Storage.Act[Type].NeedSave)
		{
			return Type;
		}
	}

	return STORAGE_MAX;
}

static void Storage_Save_Handler(void)
{
	EN_STORAGE_TYPE Type = STORAGE_MAX;
	ST_STORAGE_INFO StorageInfo;
	
	Type = Check_Storage_Save_Task();
	if(Type < STORAGE_MAX)
	{
		Storage.Act[Type].NeedSave = false;

		if(Init_Storage_Info(Type, &StorageInfo))
		{
			Storage_Write_Data_To_Flash(StorageInfo, WRITE_MAIN_FIRST);
		}
	}
}





//清除操作
void Storage_Set_NeedClear_Flag(EN_STORAGE_TYPE Type)
{
	if(Type >= STORAGE_MAX)
	{
		return;
	}

	Storage.Act[Type].NeedClear = true;
}

static EN_STORAGE_TYPE Check_Storage_Clear_Task(void)
{
	EN_STORAGE_TYPE Type = STORAGE_MAX;

	for(Type=STORAGE_UPGRADE; Type<STORAGE_MAX; Type++)
	{
		if(Storage.Act[Type].NeedClear)
		{
			return Type;
		}
	}

	return STORAGE_MAX;
}

static void Storage_Clear_Handler(void)
{
	EN_STORAGE_TYPE Type = STORAGE_MAX;
	ST_STORAGE_INFO StorageInfo;

	Type = Check_Storage_Clear_Task();
	if(Type < STORAGE_MAX)
	{
		Storage.Act[Type].NeedClear = false;

		if(Init_Storage_Info(Type, &StorageInfo))
		{
			StorageInfo.InitData();
		}
	}
}





void Storage_Handler(void)
{
	Storage_Clear_Handler();
	Storage_Save_Handler();
}




