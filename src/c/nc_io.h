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

//#ifdef __cplusplus
//extern "C" {
//#endif
	void nc_read_file(EH_String *out, char *file_path);
	void nc_get_files(EH_Array *files,const char *dir_path);
//#ifdef __cplusplus
//}
//#endif
#endif
