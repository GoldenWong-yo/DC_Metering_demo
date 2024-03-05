#include "app_tool.h"
#include <ctype.h>
#include <math.h>





u32 Hex2Ascii(u8* hex, u32 hexLen, u8* ascii)			//举例：把1个字符'1'，变成两个字符'3'和'1'
{
	u32 i;
	char chHex[] = "0123456789ABCDEF";

	for (i = 0; i<hexLen; i++)
	{
		ascii[i*2]    = (u8)(chHex[((u8)hex[i]) >> 4]);
		ascii[i*2 +1] = (u8)(chHex[((u8)hex[i]) & 0xf]);
	}

	ascii[hexLen * 2] = '\0';

	return hexLen * 2;
}

u32 Ascii2Hex(u8* ascii, u32 asciiLen, u8* hex)			//举例：把两个字符'3'和'1'，变成1个字符'1'
{
	u32 tlen, i, cnt;
	u8 t;

	for (i = 0, cnt = 0, tlen = 0; i<asciiLen; i++)
	{
		char c = toupper(ascii[i]);

		if ((c>='0'&& c<='9') || (c>='A'&& c<='F'))
		{
			t = (c >= 'A') ? c - 'A' + 10 : c - '0';

			if (cnt)
			{
				hex[tlen++] += t, cnt = 0;
			}else{
				hex[tlen] = t << 4, cnt = 1;
			}
		}
	}

	return tlen;
}

bool Check_Sum_Len(u16 len)
{
	u8 sum = 0;
	u16 temp_len = 0;

	sum += (u8)(len & 0x000f);
	sum += (u8)((len & 0x00f0) >> 4);
	sum += (u8)((len & 0x0f00) >> 8);

	sum = (~(sum % 16)) + 1;

	temp_len = (len & 0x0fff) + ((sum & 0x0f) << 12);

	if(len == temp_len)
	{
		return true;
	}else{
		return false;
	}
}

u16 Pack_Sum_Len(u16 len)
{
	u8 sum = 0;

	sum += (u8)(len & 0x000f);
	sum += (u8)((len & 0x00f0) >> 4);
	sum += (u8)((len & 0x0f00) >> 8);

	sum = (~(sum % 16)) + 1;

	len = (len & 0x0fff) + ((sum & 0x0f) << 12);
	
	return len;
}

u16 Check_Sum(u8* Buf, u16 Len)
{
	u16 idx = 0;
	u16 sum = 0;

	for(idx=0; idx<Len; idx++)
	{
		sum += Buf[idx];
	}

	return sum;
}

u16 Invert_Check_Sum(u8* Buf, u16 Len)
{
	u16 sum = 0;

	sum = Check_Sum(Buf, Len);

	return ((~sum) + 1);
}

float DataBuf_To_Float(u8* RcvData, u16* Idx, u8 FixedLen, u8 FixedPoint)
{
	u32 TempU32 = 0;
	u32 TempGain = 0;
	u8 i = 0;
	
	if(FixedPoint > 3 || (FixedLen > 4 || FixedLen == 0))
	{
		return 0.0;
	}
	
	TempGain = pow(10, FixedPoint);
	
	for(i=0; i<FixedLen; i++)
	{
		TempU32 += (RcvData[i] << ((FixedLen - 1 - i) * 8));
	}
	
	*Idx += FixedLen;
	
	return ((float)TempU32 / TempGain);
}

void Float_To_DataBuf(float Data, u8* Buf, u16* Idx, u8 FixedLen, u8 FixedPoint)
{
	u32 TempData = 0;
	u32 TempGain = 0;
	u8 i = 0;
	
	if(FixedPoint > 3 || (FixedLen > 4 || FixedLen == 0))
	{
		return;
	}
	
	TempGain = pow(10, FixedPoint);
	TempData = (u32)(Data * TempGain);
	
	for(i=0; i<FixedLen; i++)
	{
		Buf[FixedLen-1-i] = (u8)(TempData >> (i * 8));
	}
	
	*Idx += FixedLen;
}

void U16_To_Ascii(u16 U16Data,char *AsciiData)			//值转换成10进制字符：10 ->> "10"
{
	u8 PowerTime = 0;
	u16 Divisor = 0;
	u8 Temp = 0;
	u8 Idx = 0;

	if(U16Data > 9999)
	{
		PowerTime = 4;
	}
	else if(U16Data > 999)
	{
		PowerTime = 3;
	}
	else if(U16Data > 99)
	{
		PowerTime = 2;
	}
	else if(U16Data > 9)
	{
		PowerTime = 1;
	}
	else
	{
		PowerTime = 0;
	}

	Divisor = pow(10,PowerTime);
	
	while(Divisor != 0)
	{
		Temp = U16Data / Divisor;
		U16Data %= Divisor;
		Divisor /= 10;

		AsciiData[Idx++] = Temp + '0';
	}
}

static bool If_Ascii_Data_Is_U16(char *AsciiData,u16 AsciiLen)
{
	u16 Idx = 0;

	for(Idx=0;Idx<AsciiLen;Idx++)
	{
		if(AsciiData[Idx] < '0' || AsciiData[Idx] > '9')
		{
			return false;
		}
	}

	return true;
}

bool Ascii_To_U16(char *AsciiData,u16 AsciiLen,u16 *U16Data)				//字符转换10进制值："10" ->> 10
{
	u8 Idx = 0;

	if(If_Ascii_Data_Is_U16(AsciiData,AsciiLen))
	{
		*U16Data = 0;
	
		for(Idx=0;Idx<AsciiLen;Idx++)
		{
			*U16Data += (AsciiData[Idx] - '0') * pow(10,--AsciiLen);
		}
		
		return true;
	}else{
		return false;
	}
}

#if 0
typedef enum{
	MY_STRSTR_FOUND = 0,
	MY_STRSTR_NOT_FOUND
}EN_MY_STRSTR_STATE;

EN_MY_STRSTR_STATE My_Strstr(void *Str1,u16 Str1Len,void *Str2,u16 *Str1Idx)
{
	u16 Idx = 0;
	u16 Str2Len = 0;
	
	Str2Len = strlen(Str2);

	for(Idx=0;Idx<Str1Len;Idx++)
	{
		if(memcmp(&Str1[Idx],Str2,Str2Len) == 0)
		{
			*Str1Idx = Idx;
			return MY_STRSTR_FOUND;
		}
	}

	return MY_STRSTR_NOT_FOUND;
}
#endif

static const u16 crcTableAbs[] = {
	0x0000,0xcc01,0xd801,0x1400,0xF001,0x3c00,0x2800,0xe401,
	0xA001,0X6C00,0X7800,0XB401,0X5000,0X9C01,0X8801,0X4400,
};

u16 CRC16_SUM(u8 *pMsg,u16 DataLen)
{
	u16 crc_t = 0xffff;
	u16 i = 0;
	u8 chChar = 0;
	
	if(pMsg == 0)
	{
		return 0;
	}
	
	for(i=0;i<DataLen;i++)
	{
		chChar = *pMsg++;
		crc_t  = crcTableAbs[(chChar^crc_t)&15]^(crc_t>>4);
		crc_t  = crcTableAbs[((chChar>>4)^crc_t)&15]^(crc_t>>4);
	}
	
	return crc_t;
}

u16 CRC16_SUM_Continue(u8 *pMsg,u16 DataLen,u16 *crc_t)
{
	u16 i = 0;
	u8 chChar = 0;
	
	if(pMsg == NULL)
	{
		return 0;
	}
	
	for(i=0;i<DataLen;i++)
	{
		chChar   = *pMsg++;
		crc_t[0] = crcTableAbs[(chChar^crc_t[0])&15]^(crc_t[0]>>4);
		crc_t[0] = crcTableAbs[((chChar>>4)^crc_t[0])&15]^(crc_t[0]>>4);
	}
	
	return crc_t[0];
}

u8 Byte_To_Bcd(u8 Value)
{
	u8 bcdhigh = 0;

	while (Value >= 10)
	{
		bcdhigh++;
		Value -= 10;
	}

	return  ((u8)(bcdhigh << 4) | Value);
}

u8 Bcd_2_Byte(u8 Value)
{
	u8 tmp = 0U;

	tmp = ((Value >> 4) & 0x0f) * 10U;

	return (tmp + (Value & (u8)0x0f));
}

void Swap_Buf(ST_BUF* Buf1, ST_BUF* Buf2)
{
	ST_BUF TempBuf;

	TempBuf = *Buf1;
	*Buf1 = *Buf2;
	*Buf2 = TempBuf;
}


