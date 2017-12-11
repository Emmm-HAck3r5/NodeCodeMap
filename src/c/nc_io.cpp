/*
 * File: nc_io.c
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-09 18:55:06
 * ------
 * Last Modified: 2017-12-09 18:55:06
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#include "nc_io.h"

NC_File* nc_file_init(void)
{
	NC_File *fp = (NC_File*)malloc(sizeof(NC_File));
	fp->file = eh_string_init(4096);
	fp->file_ptr = 0;
	return fp;
}
void nc_read_file(NC_File *file, const char *file_path)
{
	FILE *fp = fopen(file_path, "rb");
	if (fp)
	{
		file->file_ptr = 0;
		unsigned long size;
		char *buffer;
		size_t flag;
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		rewind(fp);
		buffer = (char*)malloc(sizeof(char)*(size+1));
		if (buffer)
		{
			flag = fread(buffer, 1, size, fp);
			buffer[size] = '\0';
			if (flag == size)
			{
				eh_utf8_to_utf32(file->file, (u8*)buffer);
			}
		}
	}
}

void nc_get_cfiles(EH_Array *files,const char *dir_path)
{
	char dirbuf[4000];
	strcpy(dirbuf, dir_path);
	strcat(dirbuf, "\\*.*");
	int handle;
	_finddata_t fd;
	handle = _findfirst(dirbuf, &fd);
	if (handle == -1)
		return;
	do
	{
		if (fd.attrib&_A_SUBDIR)
		{
			if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
				continue;
			strcpy(dirbuf, dir_path);
			strcat(dirbuf, "\\");
			strcat(dirbuf, fd.name);
			nc_get_cfiles(files, dirbuf);
		}
		else
		{
			if (fd.name[strlen(fd.name) - 1] == 'c' || fd.name[strlen(fd.name) - 1] == 'h')
			{
				char *file_name = (char*)malloc(sizeof(char) * 512);
				strcpy(file_name, dir_path);
				strcat(file_name, "\\");
				strcat(file_name, fd.name);
				eh_array_append(files, file_name);
			}
		}
	} while (_findnext(handle, &fd) == 0);
	_findclose(handle);
}

u32 nc_getch(NC_File *fp)
{
	if(fp->file_ptr<fp->file->length)
		return fp->file->value[fp->file_ptr++];
	fp->file_ptr++;
	return NC_FILE_EOF;
}

void nc_file_rewind(NC_File *fp)
{
	fp->file_ptr = 0;
}
void nc_ungetch(NC_File *fp)
{
	fp->file_ptr--;
}