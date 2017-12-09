/*
 * File: eh_array.c
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-08 13:41:15
 * ------
 * Last Modified: 2017-12-08 14:14:35
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#include "eh_array.h"

EH_Array* eh_array_init(int size)
{
	EH_Array *arr = (EH_Array*)malloc(sizeof(EH_Array));
	if (arr)
	{
		arr->elmcount = 0;
		arr->buffersize = size;
		arr->data = (void**)malloc(sizeof(void*)*size);
		//todo 当malloc返回null时的动作
		memset(arr->data, 0, sizeof(void*)*size);
	}
	return arr;
}
void eh_array_free(EH_Array *arr)
{

		free(arr->data);
		free(arr);
}
void eh_array_realloc(EH_Array *arr, int newsize)
{
	int temp;
	void **p;
	temp = arr->buffersize;
	while (temp < newsize)
		temp *= 2;
	p = (void**)realloc(arr->data, temp * sizeof(void*));
	if (p)
	{
		arr->buffersize = temp;
		arr->data = p;
	}
}
void eh_array_append(EH_Array *arr, void *elm)
{
	arr->elmcount++;
	if (arr->elmcount > arr->buffersize)
		EH_Array_realloc(arr, arr->elmcount);
	arr->data[arr->elmcount - 1] = elm;
}
int eh_array_count(EH_Array *arr)
{
	return arr->elmcount;
}
void* eh_array_delete(EH_Array *arr)
{
	if (arr->elmcount > 0)
	{
		arr->elmcount--;
		return arr->data[arr->elmcount];
	}
	else
		return NULL;
}