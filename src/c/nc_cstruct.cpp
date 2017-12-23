#include "nc_cstruct.h"

NC_CFile* nc_cfile_init(NC_File *fp)
{
	NC_CFile *file = (NC_CFile*)malloc(sizeof(NC_CFile));
	file->function_list =(NC_CFunction*)malloc(sizeof(NC_CFunction));
	file->macro_list = (NC_CMacro*)malloc(sizeof(NC_CMacro));
	file->type_list = (NC_CType*)malloc(sizeof(NC_CType));
	file->var_list = (NC_CVariable*)malloc(sizeof(NC_CVariable));
	file->comp_info = nc_ccompinfo_init();
	file->token_stream = (NC_CTokenStream*)malloc(sizeof(NC_CTokenStream));
	file->token_stream->stream = nc_ctoken_generate(CTK_NULL, NULL, 0);
	file->token_stream->pos = file->token_stream->stream;
	file->parent = NULL;
	file->pfile = NULL;
	__EH_DLIST_INIT(file->token_stream->stream, next, prev);
	__EH_DLIST_INIT(file->var_list, next, prev);
	__EH_DLIST_INIT(file->type_list, next, prev);
	__EH_DLIST_INIT(file->macro_list, next, prev);
	__EH_DLIST_INIT(file->function_list, next, prev);
	//__EH_DLIST_INIT(file, rchild, lchild);
	strcpy(file->comp_info->file_path,fp->path);
	file->include_arr = eh_array_init(300);
	return file;
}

NC_Include* nc_include_init()
{
	NC_Include *icl = (NC_Include*)malloc(sizeof(NC_Include));
	icl->name = (char*)malloc(1000);
	memset(icl->name, 0, 1000);
	icl->path = (char*)malloc(1000);
	memset(icl->path, 0, 1000);
	icl->type = NULL;
	return icl;
}
NC_CCompInfo* nc_ccompinfo_init(void)
{
	NC_CCompInfo *ci = (NC_CCompInfo*)malloc(sizeof(NC_CCompInfo));
	ci->decl = eh_string_init(64);
	ci->file_path = (char*)malloc(1000);
	memset(ci->file_path, 0, 1000);
	return ci;
}

NC_CType* nc_ctype_init(void)
{
	NC_CType *type = (NC_CType*)malloc(sizeof(NC_CType));
	type->type_modifier = 0;
	type->type_type = 0;
	type->member = NULL;
	type->comp_info = nc_ccompinfo_init();
	type->type_name = eh_string_init(64);
	type->next = type->prev = NULL;
	return type;
}

NC_CVariable* nc_cvariable_init(void)
{
	NC_CVariable *var = (NC_CVariable*)malloc(sizeof(NC_CVariable));
	var->type_modifier = 0;
	var->comp_info = nc_ccompinfo_init();
	var->var_type = nc_ctype_init();
	var->var_name = eh_string_init(64);
	var->next = var->prev = NULL;
	return var;
}

NC_CFunction* nc_cfunction_init(void)
{
	NC_CFunction *func = (NC_CFunction*)malloc(sizeof(NC_CFunction));
	func->func_name = eh_string_init(64);
	func->func_body = eh_string_init(64);
	func->func_type = 0;
	func->comp_info = nc_ccompinfo_init();
	func->func_ret_type = nc_ctype_init();
	func->parameter = (NC_CVariable*)malloc(sizeof(NC_CVariable));
	__EH_DLIST_INIT(func->parameter, next, prev);
	return func;
}

