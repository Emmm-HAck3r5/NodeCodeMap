#ifndef NC_FILEDEP_H
#define NC_FILEDEP_H

#include"eh_array.h"
#include"eh_string.h"
#include"nc_io.h"
#include"nc_clex.h"



#define NC_STD 1
#define NC_CST 2

void *nc_file_dep_generate(const char *dir_path);
NC_CFile *nc_cfile_search(char *name);

#endif