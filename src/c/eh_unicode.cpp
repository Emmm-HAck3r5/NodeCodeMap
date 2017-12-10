/*
 * File: eh_unicode.c
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-08 11:29:09
 * ------
 * Last Modified: 2017-12-08 14:15:54
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#include "eh_unicode.h"

void eh_utf8_to_utf32(EH_String *dest, u8 *utf8str)
{//TODO 大小端问题
	if (utf8str&&dest)
	{
		u32 ucs;
		u8 *byte = utf8str;
		while (*byte)
		{
			ucs = 0;
			int i;
			if (*byte < 0x80)
			{//1字节
				ucs = *byte++;
			}
			else if (*byte >= 0xC0 && *byte < 0xE0)
			{//2字节
				ucs = (u32)(*byte++ & 0x1F)<<6;
				if (*byte >= 0x80)//第二字节有效
					ucs |= (u32)(*byte++ & 0x3F);
			}
			else if (*byte < 0xF0)
			{//3字节
				ucs = (u32)(*byte++ & 0xF) << 12;
				for (i = 0; i < 2; i++)
				{
					if (*byte >= 0x80)
						ucs |= (u32)(*byte++ & 0x3F) << ((1 - i) * 6);
				}
			}
			else if (*byte < 0xF8)
			{//4字节
				ucs = (u32)(*byte++ & 0x7) << 18;
				for (i = 0; i < 3; i++)
				{
					if (*byte >= 0x80)
						ucs |= (u32)(*byte++ & 0x3F) << ((2 - i) * 6);
				}
			}
			else if (*byte < 0xFC)
			{//5字节
				ucs = (u32)(*byte++ & 0x3) << 24;
				for (i = 0; i < 4; i++)
				{
					if (*byte >= 0x80)
						ucs |= (u32)(*byte++ & 0x3F) << ((3 - i) * 6);
				}
			}
			else if (*byte < 0xFE)
			{//6字节
				ucs = (u32)(*byte++ & 0x1) << 30;
				for (i = 0; i < 5; i++)
				{
					if (*byte >= 0x80)
						ucs |= (u32)(*byte++ & 0x3F) << ((4 - i) * 6);
				}
			}
			eh_string_appendc(dest, ucs, 1);
		}
	}
}