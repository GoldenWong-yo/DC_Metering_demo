#ifndef _APP_STORAGE_H_
#define _APP_STORAGE_H_

#include "app_storage_config.h"





void Init_Storage_Variables(void);
void Storage_Init(void);

void Storage_Set_NeedSave_Flag(EN_STORAGE_TYPE Type);
EN_STORAGE_TYPE Check_Storage_Save_Task(void);

void Storage_Set_NeedClear_Flag(EN_STORAGE_TYPE Type);
void Storage_Handler(void);

#endif


