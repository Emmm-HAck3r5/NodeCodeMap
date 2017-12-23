

#include "pythonH.h"


NC_CFile *nc_cfile_init()
{
	NC_CFile *file = (NC_CFile*)malloc(sizeof(NC_CFile));
	file->parent = file;
	file->file_path = (char*)malloc(1000);
	memset(file->file_path, 0, 1000);
	file->type = 0;
	file->type_modifier = 0;
	file->lineno = 0;
	file->value = (u32*)malloc(100);
	memset(file->value, 0, 100);
	file->data = (void**)malloc(100);
	memset(file->data,0,100);
	return file;
}

NC_CFile *Prase()
{
	NC_CFile *cfile1 = nc_cfile_init();
	NC_CFile *cfile2 = nc_cfile_init();
	cfile1->parent = cfile2;
	cfile2->parent = NULL;
	strcpy(cfile1->file_path, "D:\\qvodplayer\\qvod.c");
	strcpy(cfile2->file_path,"D:\\xfplayer\\xfplayer.h");
	cfile1->type = 0x2;
	cfile2->type = 0x1;
	return cfile1;
}