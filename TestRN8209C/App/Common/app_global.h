#ifndef _APP_GLOBAL_H_
#define _APP_GLOBAL_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "stm32f1xx_hal.h"





//#pragma pack(1)


typedef enum{
	BOOT_VERSION = 0x01,
	BOOT_WAIT_UPGRADE_FG = 0x02,		//实际上没有这个版本，这个标志主要用来标记程序卡在bootloader，在等待升级
	APP_VERSION = 0x10,
}EN_SOFTWARE_VERSION;

#define ENABLE_UPGRADE 0

#if ENABLE_UPGRADE
#define STM32_APP_START_ADDR 0x8007000
#define STM32_APP_END_ADDR   0x8020000
#else
#define STM32_APP_START_ADDR 0x8000000
#define STM32_APP_END_ADDR   0x8019000
#endif


typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef volatile int64_t vs64;
typedef volatile int32_t vs32;
typedef volatile int16_t vs16;
typedef volatile int8_t  vs8;

typedef volatile uint64_t vu64;
typedef volatile uint32_t vu32;
typedef volatile uint16_t vu16;
typedef volatile uint8_t  vu8;


#define TIME_0MS   0
#define TIME_1MS   1
#define TIME_2MS   2
#define TIME_3MS   3
#define TIME_4MS   4
#define TIME_5MS   5
#define TIME_10MS  10
#define TIME_15MS  15
#define TIME_20MS  20
#define TIME_25MS  25
#define TIME_30MS  30
#define TIME_40MS  40
#define TIME_50MS  50
#define TIME_90MS  90
#define TIME_100MS 100
#define TIME_200MS 200
#define TIME_300MS 300
#define TIME_500MS 500
#define TIME_600MS 600
#define TIME_700MS 700
#define TIME_1S    1000
#define TIME_2S    2000
#define TIME_3S    3000
#define TIME_5S    5000
#define TIME_6S    6000
#define TIME_10S   10000
#define TIME_12S   12000
#define TIME_15S   15000
#define TIME_20S   20000
#define TIME_30S   30000
#define TIME_1MIN  60000
#define TIME_3MIN  180000
#define TIME_5MIN  300000
#define TIME_10MIN 600000
#define TIME_15MIN 900000
#define TIME_30MIN 1800000
#define TIME_1H    3600000
#define TIME_8H    28800000


typedef struct{
	s32 Int;
	float Float;
}ST_MY_FLOAT;

typedef struct{
	u8 *pBuf;
	u16 Len;
}ST_BUF;

typedef enum{
	Status_Success = 0,
	Status_Enable,
	Status_Wait,
	Status_Error,
	Status_Timeout,
	Status_ProtocolError,
}EN_Global_Status;


void My_System_Init(void);
void Debug_MCU_Freeeze(void);
void Reset_Tick(u32 *LastTick);
bool Tick_Timeout(u32 *LastTick, u32 TickTimes);

#endif
