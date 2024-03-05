#include "app_storage_drv.h"





typedef struct{
	ST_STORAGE_CHECK StorageCheck;
	u8 DataBuf[W25QXX_SECTOR_SIZE];
}ST_STORAGE_DATA;

typedef struct{
	u32 MainAddr;
	u32 BackupAddr;
}ST_STORAGE_ADDR;

static ST_STORAGE_ADDR StorageAddr;


static void Init_Storage_Addr(ST_STORAGE_INFO StorageInfo)
{
	StorageAddr.MainAddr   = StorageInfo.Type * W25QXX_SECTOR_SIZE;
	StorageAddr.BackupAddr = (W25qxxInfo.HalfSectorIdx + StorageInfo.Type) * W25QXX_SECTOR_SIZE;
}

void Storage_Write_Data_To_Flash(ST_STORAGE_INFO StorageInfo, EN_WRITE_FLASH_ORDER WriteOrder)
{
	//ST_STORAGE_DATA TempStorageData;
	
	//memcpy(&TempStorageData,StorageInfo.RamAddr,StorageInfo.Len);			//将内存拷贝出来，避免在存储的过程中，有中断对内存上的数据进行操作
	
	Init_Storage_Addr(StorageInfo);
	
	StorageInfo.RamAddr->StorageCheck.InitFlag = W25QXX_SECTOR_INIT_FLAG;
	StorageInfo.RamAddr->StorageCheck.FlashLen = StorageInfo.Len;
	StorageInfo.RamAddr->StorageCheck.CheckSum = 0;
	StorageInfo.RamAddr->StorageCheck.CheckSum = Check_Sum((u8*)StorageInfo.RamAddr, StorageInfo.Len);
	
	switch(WriteOrder)
	{
		case WRITE_MAIN_FIRST:
			W25qxx_Write(StorageAddr.MainAddr,   (u8*)StorageInfo.RamAddr, StorageInfo.Len);
			W25qxx_Write(StorageAddr.BackupAddr, (u8*)StorageInfo.RamAddr, StorageInfo.Len);
			break;
		case WRITE_BACKUP_FIRST:
			W25qxx_Write(StorageAddr.BackupAddr, (u8*)StorageInfo.RamAddr, StorageInfo.Len);
			W25qxx_Write(StorageAddr.MainAddr,   (u8*)StorageInfo.RamAddr, StorageInfo.Len);
			break;
		case WRITE_MAIN_ONLY:
			W25qxx_Write(StorageAddr.MainAddr,   (u8*)StorageInfo.RamAddr, StorageInfo.Len);
			break;
		case WRITE_BACKUP_ONLY:
			W25qxx_Write(StorageAddr.BackupAddr, (u8*)StorageInfo.RamAddr, StorageInfo.Len);
			break;
	}
}

static void Storage_Check_Back_Area_1(ST_STORAGE_INFO StorageInfo)			//备份区间
{
	u16 TempFlashLen = 0;
	u16 TempFlashCheckSum = 0;
	
	W25qxx_Read(StorageAddr.BackupAddr, (u8*)StorageInfo.RamAddr, StorageInfo.Len);
	if(StorageInfo.RamAddr->StorageCheck.InitFlag != W25QXX_SECTOR_INIT_FLAG)
	{
		StorageInfo.InitData();
	}else{
		if(StorageInfo.RamAddr->StorageCheck.FlashLen < StorageInfo.Len)
		{
			TempFlashLen                               = StorageInfo.RamAddr->StorageCheck.FlashLen;
			TempFlashCheckSum                          = StorageInfo.RamAddr->StorageCheck.CheckSum;
			StorageInfo.RamAddr->StorageCheck.CheckSum = 0;
			StorageInfo.RamAddr->StorageCheck.CheckSum = Check_Sum((u8*)StorageInfo.RamAddr, TempFlashLen);
			if(TempFlashCheckSum != StorageInfo.RamAddr->StorageCheck.CheckSum)
			{
				StorageInfo.InitData();
			}else{
				Storage_Write_Data_To_Flash(StorageInfo, WRITE_MAIN_FIRST);
			}
		}
		else if(StorageInfo.RamAddr->StorageCheck.FlashLen > StorageInfo.Len)
		{
			StorageInfo.InitData();
		}else{
			TempFlashCheckSum                          = StorageInfo.RamAddr->StorageCheck.CheckSum;
			StorageInfo.RamAddr->StorageCheck.CheckSum = 0;
			StorageInfo.RamAddr->StorageCheck.CheckSum = Check_Sum((u8*)StorageInfo.RamAddr, StorageInfo.Len);
			if(TempFlashCheckSum != StorageInfo.RamAddr->StorageCheck.CheckSum)
			{
				StorageInfo.InitData();
			}else{
				Storage_Write_Data_To_Flash(StorageInfo, WRITE_MAIN_ONLY);
			}
		}
	}
}

static void Storage_Check_Back_Area_2(ST_STORAGE_INFO StorageInfo)
{
	u16 TempFlashCheckSum = 0;
	ST_STORAGE_DATA TempStorageData;
	
	W25qxx_Read(StorageAddr.BackupAddr, (u8*)&TempStorageData, StorageInfo.Len);
	if(TempStorageData.StorageCheck.InitFlag != W25QXX_SECTOR_INIT_FLAG)
	{
		Storage_Write_Data_To_Flash(StorageInfo, WRITE_BACKUP_FIRST);
	}else{
		if(TempStorageData.StorageCheck.FlashLen != StorageInfo.Len)
		{
			Storage_Write_Data_To_Flash(StorageInfo, WRITE_BACKUP_FIRST);
		}else{
			TempFlashCheckSum                     = TempStorageData.StorageCheck.CheckSum;
			TempStorageData.StorageCheck.CheckSum = 0;
			TempStorageData.StorageCheck.CheckSum = Check_Sum((u8*)&TempStorageData, StorageInfo.Len);
			if(TempFlashCheckSum != TempStorageData.StorageCheck.CheckSum)
			{
				Storage_Write_Data_To_Flash(StorageInfo, WRITE_BACKUP_FIRST);
			}else{
				memcpy((u8*)StorageInfo.RamAddr, (u8*)&TempStorageData, StorageInfo.Len);
				Storage_Write_Data_To_Flash(StorageInfo, WRITE_MAIN_ONLY);
			}
		}
	}
}

static void Storage_Check_Back_Area_3(ST_STORAGE_INFO StorageInfo)
{
	u16 TempFlashCheckSum = 0;
	ST_STORAGE_DATA TempStorageData;
	
	W25qxx_Read(StorageAddr.BackupAddr, (u8*)&TempStorageData, StorageInfo.Len);
	if(TempStorageData.StorageCheck.InitFlag != W25QXX_SECTOR_INIT_FLAG)
	{
		Storage_Write_Data_To_Flash(StorageInfo, WRITE_BACKUP_ONLY);
	}else{
		if(TempStorageData.StorageCheck.FlashLen != StorageInfo.Len)
		{
			Storage_Write_Data_To_Flash(StorageInfo, WRITE_BACKUP_ONLY);
		}else{
			TempFlashCheckSum                     = TempStorageData.StorageCheck.CheckSum;
			TempStorageData.StorageCheck.CheckSum = 0;
			TempStorageData.StorageCheck.CheckSum = Check_Sum((u8*)&TempStorageData, StorageInfo.Len);
			if(TempFlashCheckSum != TempStorageData.StorageCheck.CheckSum)
			{
				Storage_Write_Data_To_Flash(StorageInfo, WRITE_BACKUP_ONLY);
			}
		}
	}
}

void Storage_Check_Main_Area(ST_STORAGE_INFO StorageInfo)
{
	u16 TempFlashLen = 0;
	u16 TempFlashCheckSum = 0;
	
	Init_Storage_Addr(StorageInfo);
	
	W25qxx_Read(StorageAddr.MainAddr, (u8*)StorageInfo.RamAddr, StorageInfo.Len);
	if(StorageInfo.RamAddr->StorageCheck.InitFlag != W25QXX_SECTOR_INIT_FLAG)
	{
		Storage_Check_Back_Area_1(StorageInfo);
	}else{
		if(StorageInfo.RamAddr->StorageCheck.FlashLen < StorageInfo.Len)
		{
			TempFlashLen                               = StorageInfo.RamAddr->StorageCheck.FlashLen;
			TempFlashCheckSum                          = StorageInfo.RamAddr->StorageCheck.CheckSum;
			StorageInfo.RamAddr->StorageCheck.CheckSum = 0;
			StorageInfo.RamAddr->StorageCheck.CheckSum = Check_Sum((u8*)StorageInfo.RamAddr, TempFlashLen);
			if(TempFlashCheckSum != StorageInfo.RamAddr->StorageCheck.CheckSum)
			{
				Storage_Check_Back_Area_1(StorageInfo);
			}else{
				Storage_Check_Back_Area_2(StorageInfo);
			}
		}
		else if(StorageInfo.RamAddr->StorageCheck.FlashLen > StorageInfo.Len)
		{
			Storage_Check_Back_Area_1(StorageInfo);
		}else{
			TempFlashCheckSum                          = StorageInfo.RamAddr->StorageCheck.CheckSum;
			StorageInfo.RamAddr->StorageCheck.CheckSum = 0;
			StorageInfo.RamAddr->StorageCheck.CheckSum = Check_Sum((u8*)StorageInfo.RamAddr, StorageInfo.Len);
			if(TempFlashCheckSum != StorageInfo.RamAddr->StorageCheck.CheckSum)
			{
				Storage_Check_Back_Area_1(StorageInfo);
			}else{
				Storage_Check_Back_Area_3(StorageInfo);
			}
		}
	}
}





