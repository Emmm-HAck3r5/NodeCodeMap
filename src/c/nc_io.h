/*
 * File: nc_io.h
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-09 18:55:00
 * ------
 * Last Modified: 2017-12-09 18:55:01
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#ifndef NC_IO_H
#define NC_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "eh_string.h"
#include "eh_array.h"
#include "eh_unicode.h"

#define NC_FILE_EOF 0x0
typedef struct NC_File
{
	EH_String *file;
	u32 file_ptr;
}NC_File;
//#ifdef __cplusplus
//extern "C" {
//#endif
	NC_File* nc_file_init(void);
	void nc_read_file(NC_File *file, char *file_path);
	void nc_get_cfiles(EH_Array *files,const char *dir_path);
	u32 nc_getch(NC_File *fp);
	void nc_file_rewind(NC_File *fp);
	void nc_ungetch(NC_File *fp);
//#ifdef __cplusplus
//}
//#endif
#endif
