/*
 * File: eh_array.h
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-08 13:44:43
 * ------
 * Last Modified: 2017-12-08 14:17:03
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#ifndef EH_ARRAY_H
#define EH_ARRAY_H

#include <stdlib.h>
#include <string.h>
#include "eh_typedef.h"

typedef struct EH_Array
{
	int elmcount;
	int buffersize;
	void **data;
}EH_Array;

#ifdef __cplusplus
extern "C" {
#endif
	EH_Array* eh_array_init(int size);
	void eh_array_free(EH_Array *arr);
	void eh_array_realloc(EH_Array *arr, int newsize);
	void eh_array_append(EH_Array *arr, void *elm);
	int eh_array_count(EH_Array *arr);
	void* eh_array_delete(EH_Array *arr);
#ifdef __cplusplus
}
#endif
#endif