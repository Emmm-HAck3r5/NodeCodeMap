#ifndef PYTHON_NCC_H
#define PYTHON_NCC_H


#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>
#include <stdbool.h>
#include "eh_typedef.h"

typedef enum NC_CFileType
{
	NC_CFile_H = 0x1, NC_CFile_C = 0x2,
	NC_CFile_STD = 0x10
}NC_CFileType;

typedef struct StructPointerTest
{
	char name[20];
	int age;
}StructPointerTest, *StructPointer;

typedef struct NC_CFile
{
	struct NC_CFile *parent;
	char *file_path;
	int type;
	u8 type_modifier;
	u32 lineno;
	u32 *value;
	void **data;
}NC_CFile;

NC_CFile *nc_cfile_init();

extern "C" __declspec(dllexport) NC_CFile *Prase();

#endif // !PYTHON_NCC_H
