#include "nc_dock.h"

NC_CFile *nc_dock_generate(const char *dir)
{
	nc_file_dep_generate(dir);
	int i;
	NC_CFile *f =(NC_CFile*)malloc(sizeof(NC_CFile));
	for (i = 0; i < cfile_array->elmcount; i++)
	{
		f = (NC_CFile*)cfile_array->data[i];
		nc_parse_translation_unit(f);
		nc_parse_get_decl(f);
	}
	return file_list;
}