#include "app_test.h"
#include "app_global.h"
#include "app_printf.h"





ST_TEST Test;


void Init_Test_Variables(void)
{
	memset(&Test, 0, sizeof(Test));
}

bool Start_Test(void)
{
	if(Test.State != TEST_DISABLE)
	{
		Test.State = TEST_TESTING;
		
		return true;
	}
	
	return false;
}

bool End_Test(void)
{
	Test.State = TEST_DISABLE;
	
	return true;
}

void Check_Test_Valid(void)
{
	if(Test.State != TEST_DISABLE)
	{
		if(Test.State == TEST_ENABLE)						//开机后默认测试功能是使能的
		{
			if(Tick_Timeout(&Test.StartTick, TIME_10S))
			{
				End_Test();									//10秒钟后，测试功能失效
			}
		}else{												//如果正在进行测试
			if(Tick_Timeout(&Test.EndTick, TIME_1MIN))
			{
				End_Test();									//1分钟后，测试功能自动关闭
			}
		}
	}
}



