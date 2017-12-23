
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

typedef enum NC_CTypeModifier
{
	NC_CTypeModifier_Const = 0x1,
	NC_CTypeModifier_Static = 0x2,
	NC_CTypeModifier_Extern = 0x4,
	NC_CTypeModifier_Register = 0x8,
	NC_CTypeModifier_Volatile = 0x10,
	NC_CTypeModifier_Auto = 0x20
}NC_CTypeModifier;

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
    int *value;
    void **data;
}NC_CFile;

NC_CFile* Parse()
{
    NC_CFile* file = (NC_CFile*)malloc(sizeof(NC_CFile));
    file->parent = (NC_CFile*)malloc(sizeof(NC_CFile));
    file->parent->type = 10;
    strcpy(file->file_path, "Joe");
    file->type = NC_CFile_STD;
    file->type_modifier = NC_CTypeModifier_Auto;
    file->lineno = NC_CTypeModifier_Auto;
    file->value = &(file->lineno);
    void* d = &(file->lineno);
    file->data = d;
    return file;
}