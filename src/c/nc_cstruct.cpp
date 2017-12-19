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
	__EH_DLIST_INIT(file->token_stream->stream, next, prev);
	__EH_DLIST_INIT(file->var_list, next, prev);
	__EH_DLIST_INIT(file->type_list, next, prev);
	__EH_DLIST_INIT(file->macro_list, next, prev);
	__EH_DLIST_INIT(file->function_list, next, prev);
	__EH_DLIST_INIT(file, rchild, lchild);
	eh_string_copy(file->comp_info->file_data, fp->file);
	strcpy(file->comp_info->file_path,fp->path);
	strcpy(file->comp_info->file_name, fp->path);
	while (strchr(file->comp_info->file_name, '\\') != 0)
	{
		file->comp_info->file_name++;
	}
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
	icl->ptrs = eh_array_init(300);
	return icl;
}
NC_CCompInfo* nc_ccompinfo_init(void)
{
	NC_CCompInfo *ci = (NC_CCompInfo*)malloc(sizeof(NC_CCompInfo));
	ci->decl = eh_string_init(64);
	ci->file_data = eh_string_init(128);
	ci->file_path = (char*)malloc(1000);
	memset(ci->file_path, 0, 1000);
	ci->file_name = (char*)malloc(1000);
	memset(ci->file_name, 0, 1000);
	return ci;
}

NC_CFile *nc_cfile_search(char *name)//╡Иурнд╪Ч
{
	//printf("searching: \"%s\"\n",name);
	for (int i = 0; i < cfile_array->elmcount; i++)
	{
		//printf("checking: \"%s\"\nresult: ", ((NC_CFile*)cfile_array->data[i])->comp_info->file_name);
		if (strcmp(((NC_CFile*)cfile_array->data[i])->comp_info->file_name, name)==0)
		{
			//printf("match: %s\n\n", ((NC_CFile*)cfile_array->data[i])->comp_info->file_name);
			return (NC_CFile*)cfile_array->data[i];
		}
		//printf("unmatch\n");
	}
	//printf("failed\n\n");
	return NULL;
}