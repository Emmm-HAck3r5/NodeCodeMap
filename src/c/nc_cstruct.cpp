#include "nc_cstruct.h"

NC_CFile* nc_cfile_init(NC_File *fp)
{
	NC_CFile *file = (NC_CFile*)malloc(sizeof(NC_CFile));
	file->function_list =(NC_CFunction*)malloc(sizeof(NC_CFunction));
	file->macro_list = (NC_CMacro*)malloc(sizeof(NC_CMacro));
	file->type_list = (NC_CType*)malloc(sizeof(NC_CType));
	file->var_list = (NC_CVariable*)malloc(sizeof(NC_CVariable));
	file->comp_info = nc_ccompinfo_init();
	
	file->token_stream = nc_ctoken_generate(CTK_NULL, NULL, 0);
	file->parent = NULL;
	__EH_DLIST_INIT(file->token_stream, next, prev);
	__EH_DLIST_INIT(file->var_list, next, prev);
	__EH_DLIST_INIT(file->type_list, next, prev);
	__EH_DLIST_INIT(file->macro_list, next, prev);
	__EH_DLIST_INIT(file->function_list, next, prev);
	__EH_DLIST_INIT(file, rchild, lchild);
	eh_string_copy(file->comp_info->file_path, fp->file);
	return file;
}

NC_CCompInfo* nc_ccompinfo_init(void)
{
	NC_CCompInfo *ci = (NC_CCompInfo*)malloc(sizeof(NC_CCompInfo));
	ci->decl = eh_string_init(64);
	ci->file_path = eh_string_init(128);
	return ci;
}