/*
 * File: eh_string.h
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-08 10:18:35
 * ------
 * Last Modified: 2017-12-08 14:16:37
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#ifndef EH_STRING_H
#define EH_STRING_H

#include <stdlib.h>
#include <string.h>

#include "eh_typedef.h"
#define EH_STRING_NPOS -1

//unicode 字符串
typedef struct EH_String
{
    unsigned int length;
    unsigned int buffersize;
	u32 *value;
}EH_String;

#ifdef __cplusplus
extern "C"{
#endif
    EH_String* eh_string_init(unsigned int size);
    void eh_string_free(EH_String *str);
    void eh_string_clear(EH_String *str);
    void eh_string_realloc(EH_String *str,unsigned int newsize);
	void eh_string_appendc(EH_String *str, u32 c, int n);
	u32 *eh_string_tostring(EH_String *str);
	int eh_string_findc(EH_String *str, u32 c);
	void eh_string_substr(EH_String *dest, EH_String *str, unsigned int begin, int end);
	int eh_string_copy(EH_String *dest, const EH_String *src);
	void eh_string_reverse(EH_String *str);

#ifdef __cplusplus
}
#endif
#endif