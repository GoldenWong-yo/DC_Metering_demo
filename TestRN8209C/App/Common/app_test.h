#ifndef _APP_TEST_H_
#define _APP_TEST_H_

#include "app_global.h"





typedef enum{
	TEST_ENABLE = 0,		//測試功能有效
	TEST_DISABLE,			//測試功能無效
	TEST_TESTING			//測試中
}EN_TEST_STATE;

typedef struct{
	EN_TEST_STATE State;
	u32 StartTick;
	u32 EndTick;
}ST_TEST;

extern ST_TEST Test;


void Init_Test_Variables(void);
bool Start_Test(void);
bool End_Test(void);
void Check_Test_Valid(void);


#endif

