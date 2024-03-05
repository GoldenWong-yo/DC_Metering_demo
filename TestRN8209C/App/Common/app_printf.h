#ifndef _APP_PRINTF_H_
#define _APP_PRINTF_H_





#define PRINTF_DEBUG 0

#if PRINTF_DEBUG
#define My_Printf(format, ...)   printf(format "\r\n",##__VA_ARGS__)
#define Printf_Info(format, ...)   printf("【main】info:\r\n" format "\r\n",##__VA_ARGS__)
#define Printf_Debug(format, ...)  printf("【main】debug:\r\n" format "\r\n",##__VA_ARGS__)
#define Printf_Error(format, ...)  printf("【main】error:\r\n" format "\r\n",##__VA_ARGS__)
#else
#define My_Printf(format, ...)
#define Printf_Info(format, ...)
#define Printf_Debug(format, ...)
#define Printf_Error(format, ...)
#endif


#endif

