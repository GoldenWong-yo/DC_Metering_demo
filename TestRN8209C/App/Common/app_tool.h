#ifndef _APP_TOOL_H_
#define _APP_TOOL_H_

#include "app_global.h"




u32 Hex2Ascii(u8* hex, u32 hexLen, u8* ascii);
u32 Ascii2Hex(u8* ascii, u32 asciiLen, u8* hex);
bool Check_Sum_Len(u16 len);
u16 Pack_Sum_Len(u16 len);
u16 Check_Sum(u8* Buf, u16 Len);
u16 Invert_Check_Sum(u8* Buf, u16 Len);
float DataBuf_To_Float(u8* RcvData, u16* Idx, u8 FixedLen, u8 FixedPoint);
void Float_To_DataBuf(float Data, u8* Buf, u16* Idx, u8 FixedLen, u8 FixedPoint);
void U16_To_Ascii(u16 U16Data,char *AsciiData);
bool Ascii_To_U16(char *AsciiData,u16 AsciiLen,u16 *U16Data);
u16 CRC16_SUM(u8 *pMsg,u16 DataLen);
u16 CRC16_SUM_Continue(u8 *pMsg,u16 DataLen,u16 *crc_t);
u8 Byte_To_Bcd(u8 Value);
u8 Bcd_2_Byte(u8 Value);
void Swap_Buf(ST_BUF* Buf1, ST_BUF* Buf2);


#endif

