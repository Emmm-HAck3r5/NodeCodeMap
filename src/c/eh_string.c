/*
 * File: eh_string.c
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-08 10:18:42
 * ------
 * Last Modified: 2017-12-08 10:18:43
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#include "eh_string.h"

EH_String* eh_string_init(unsigned int size)
{
	EH_String *str = (EH_String*)malloc(sizeof(EH_String));
	if (str)
	{
		str->length = 0;
		str->buffersize = size;
		str->value = (unsigned int*)malloc(sizeof(u32)*size);
		if (str->value)
		{
			memset(str->value, 0, sizeof(u32)*size);
			return str;
		}
		else
			return NULL;
	}
	return NULL;
}
void eh_string_free(EH_String *str)
{
	free(str->value);
	free(str);
}
void eh_string_clear(EH_String *str)
{
	if (str)
	{
		str->length = 0;
		if (str->value)
		{
			memset(str->value, 0, sizeof(u32)*str->buffersize);
		}
	}
}
void eh_string_realloc(EH_String *str, unsigned int newsize)
{
	if (str)
	{
		unsigned int temp = str->buffersize;
		while (temp < newsize)
			temp *= 2;
		if (str->value)
		{
			str->buffersize = temp;
			str->value = (u32*)realloc(str->value, temp*sizeof(u32));
		}
	}
}
void eh_string_appendc(EH_String *str, u32 c, int n)
{
	if (str)
	{
		str->length += n;
		if (str->length >= str->buffersize)
			eh_string_realloc(str, str->length+1);
		for (; n > 0; n--)
			str->value[str->length - n] = c;
		str->value[str->length] = '\0';
	}
}
u32 *eh_string_tostring(EH_String *str)
{
	return str->value;
}
int eh_string_findc(EH_String *str, u32 c)
{
	if (str)
	{
		unsigned int n;
		for (n = 0; n < str->length; n++)
			if (str->value[n] == c)
				return n;
	}
	return EH_STRING_NPOS;
}
void eh_string_substr(EH_String *dest, EH_String *src, unsigned int begin, int end)
{
	if (src)
	{
		if (end == EH_STRING_NPOS)
			end = src->length - 1;
		if (end < src->length&& begin <= end)
		{
			unsigned int size = end - begin + 2;
			if (dest)
			{
				dest->length = size - 1;
				memcpy(dest->value, &(src->value[begin]), dest->length * sizeof(u32));
				dest->value[dest->length] = '\0';
			}
		}
	}
}
int eh_string_copy(EH_String *dest, const EH_String *src)
{
	if (src&&dest)
	{
		if (dest->buffersize < src->buffersize)
			eh_string_realloc(dest, src->buffersize);
		if (src->value&&dest->value)
			memcpy(dest->value, src->value,sizeof(u32)*dest->buffersize);
		dest->length = src->length;
		return 1;
	}
	return -1;
}

void eh_string_reverse(EH_String *str)
{
	EH_String *p;
	if (str)
	{
		p = eh_string_init(str->buffersize);
		if (p)
		{
			unsigned int i;
			for (i = 0; i < str->length; i++)
				p->value[i] = str->value[str->length - 1 - i];
			p->value[i] = '\0';
			p->length = str->length;
			eh_string_free(str);
			*str = *p;
		}
	}
}