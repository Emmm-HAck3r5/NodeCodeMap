/*
 * File: nc_cstruct.h
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-08 19:09:49
 * ------
 * Last Modified: 2017-12-10 18:07:46
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#ifndef NC_CSTRUCT_H
#define NC_CSTRUCT_H

#include <stdbool.h>
#include "eh_array.h"
#include "eh_string.h"
#include "eh_dlist.h"
#include "eh_stack.h"
#include "eh_typedef.h"
#include "nc_ctoken.h"
#include "nc_io.h"
//#include "nc_clex.h"

typedef enum NC_CFileType
{
	NC_CFile_H = 0x1, NC_CFile_C = 0x2,
	NC_CFile_STD = 0x10
}NC_CFileType;
typedef struct NC_CFile
{
	struct NC_CFile *parent;
	struct NC_CFile *lchild, *rchild;
	NC_CFileType file_type;
	struct NC_CCompInfo *comp_info;
	struct NC_CTokenStream *token_stream;
	struct NC_CFunction *function_list;
	struct NC_CVariable *var_list;
	struct NC_CMacro *macro_list;
	struct NC_CType *type_list;
	EH_Array *include_arr;
}NC_CFile;

typedef struct NC_CTokenStream
{
	CToken *stream;
	CToken *pos;
}NC_CTokenStream;;

typedef struct NC_CCompInfo
{
	EH_String *file_data;
	char *file_path;
	EH_String *decl;
	u32 lineno;
	u32 pos;//该变量用于获取decl，存储的是ftell返回值
}NC_CFileInfo;
typedef enum NC_CTypeType
{
	NC_CType_Basic = 0x1,
	NC_CType_Struct = 0x2,
	NC_CType_Enum = 0x4,
	NC_CType_Union = 0x8,
	NC_CType_HBasic = 0x10,
	NC_CType_Pointer = 0x20,
	NC_CType_Array = 0x40,
}NC_CTypeType;

typedef enum NC_CTypeModifier
{
	NC_CTypeModifier_Const = 0x1,
	NC_CTypeModifier_Static = 0x2,
	NC_CTypeModifier_Extern = 0x4,
	NC_CTypeModifier_Register = 0x8,
	NC_CTypeModifier_Volatile = 0x10,
	NC_CTypeModifier_Auto = 0x20
}NC_CTypeModifier;
typedef struct NC_CType
{
	struct NC_CType *next, *prev;
	u8 type_type;
	EH_String *type_name;
	NC_CCompInfo *comp_info;
	u8 type_modifier;//此处const指内容是否是const
	struct NC_CVariable *member;
}NC_CType;

typedef struct NC_CVariable
{
	struct NC_CVariable *next, *prev;
	NC_CType var_type;
	EH_String *var_name;
	NC_CCompInfo *comp_info;
	u8 type_modifier;//此处const指指针是否是const
}NC_CVariable;

typedef enum NC_CFunctionType
{
	NC_CFunc_stdcall = 0x1,
	NC_CFunc_cdecl = 0x2,
	NC_CFunc_fastcall = 0x4,
	NC_CFunc_extern = 0x8,
	NC_CFunc_static = 0x10,
	NC_CFunc_inline = 0x20
}NC_CFunctionType;

typedef struct NC_CFunction
{
	struct NC_CFunction *next, *prev;
	EH_String *func_name;
	NC_CCompInfo *comp_info;
	u8 func_type;
	NC_CType func_ret_type;
	struct NC_CVariable *parameter;
	EH_String *func_body;
}NC_CFunction;

enum Mcotype { rplace, function };

typedef struct NC_CMacro
{
	struct NC_CMacro *next, *prev;
	EH_String *macro_name;
	NC_CCompInfo *comp_info;
	EH_String *parameter;
	EH_String *after_expand;

	Mcotype macrotype = rplace;
	CToken *macro_name_tokens;
	CToken *macro_afterexpand_tokens;
	//add function pointer
}NC_CMacro;

typedef struct NC_Include
{
	char* name;
	char* path;
	int type;
}NC_Include;

NC_CFile* nc_cfile_init(NC_File *fp);
NC_CCompInfo* nc_ccompinfo_init(void);
NC_Include* nc_include_init();
#endif