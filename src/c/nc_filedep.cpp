#include "nc_filedep.h"


void *nc_file_dep_generate(const char *dir_path)
{
	EH_Array *file_array_path = eh_array_init(300);
	//EH_Array *cfile_array = eh_array_init(300);
	nc_get_cfiles(file_array_path, dir_path);
	NC_File *file=nc_file_init();
	CToken *temp_tk;
	bool include_flag = false;
	char *temp_str=NULL;
	struct NC_Include *icl_info;
	nc_lex_init();
	int n;
	for (int i = 0; i < file_array_path->elmcount; i++)
	{
		file = nc_file_init();
		//icl_info = nc_include_init();
		nc_read_file(file, (char*)file_array_path->data[i]);
		//printf("%s\n",(char*)file_array_path->data[i]);
		nc_analyze_token(file);
		__EH_DLIST_FOREACH(((NC_CFile*)cfile_array->data[i])->token_stream->stream, next, temp_tk)
		{
			//printf("%s\n",eh_string_toasciistring(temp_string,temp_tk->token_value));
			if (temp_tk->token_type == CPKW_INCLUDE)
				include_flag = true;
			else if (include_flag && temp_tk->token_type == CTK_STR)
			{
				temp_str = (char*)malloc(1000);
				memset(temp_str, 0, 1000);
				icl_info = nc_include_init();
				include_flag = false;
				icl_info->type = NC_CST;
				temp_str = eh_string_toasciistring(temp_str, temp_tk->token_value);
				temp_str++;
				if (strchr(temp_str, '\"') == NULL) break;
				n = strchr(temp_str, '\"') - temp_str;
				strncpy(icl_info->name, temp_str, n);
				while (strchr(icl_info->name, '\\') != NULL)
					icl_info->name++;
				eh_array_append(((NC_CFile*)cfile_array->data[i])->include_arr, icl_info);
				//printf("%d %s\n",((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[((NC_CFile*)cfile_array->data[i])->include_arr->elmcount-1])->type, ((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[((NC_CFile*)cfile_array->data[i])->include_arr->elmcount - 1])->name);
			}
			else
				include_flag = false;
			//else if (include_flag == 1 && temp_tk->token_type == CTK_IDENT)
			//{
			//	temp_str = (char*)malloc(1000);
			//	memset(temp_str, 0, 1000);
			//	icl_info = nc_include_init();
			//	include_flag = false;
			//	icl_info->type = NC_STD;
			//	strcpy(icl_info->name, eh_string_toasciistring(temp_str, temp_tk->token_value));
			//	eh_array_append(((NC_CFile*)cfile_array->data[i])->include_arr, icl_info);
			//	//printf("%d %s\n", ((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[((NC_CFile*)cfile_array->data[i])->include_arr->elmcount - 1])->type, ((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[((NC_CFile*)cfile_array->data[i])->include_arr->elmcount - 1])->name);
			//}
		}
	}
	temp_str = (char*)malloc(300);
	memset(temp_str, 0, 300);
	for (int i = 0; i < cfile_array->elmcount; i++)
	{
		((NC_CFile*)cfile_array->data[i])->lchild = (NC_CFile*)cfile_array->data[i];
		((NC_CFile*)cfile_array->data[i])->rchild = (NC_CFile*)cfile_array->data[i];
		((NC_CFile*)cfile_array->data[i])->parent = (NC_CFile*)cfile_array->data[i];
	}
	NC_CFile *temp_ptr;
	int flag_start=0;
	for (int i = 0; i < cfile_array->elmcount; i++)
	{
		flag_start = 0;
		temp_ptr = (NC_CFile*)cfile_array->data[i];
		if ((((NC_CFile*)cfile_array->data[i])->include_arr->elmcount) == 0)
		{
			//printf("ERROR 1 NULL");
			continue;
		}
		/*while ((nc_cfile_search(((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[flag_start])->name) == NULL)&&flag_start<((NC_CFile*)cfile_array->data[i])->include_arr->elmcount-2)
			flag_start++;
		if (flag_start<)
		printf("STEP 1");
		printf("%d\n", (NC_Include*)(NC_CFile*)cfile_array);
		printf("%d\n", (NC_Include*)(NC_CFile*)cfile_array->data[i]);
		printf("%d\n", (NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr);
		printf("%d\n", ((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[flag_start]));
		*/
		temp_ptr->lchild = nc_cfile_search(((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[flag_start])->name);
		if (nc_cfile_search(((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[flag_start])->name) == NULL)
			break;
		temp_ptr->lchild->parent = temp_ptr;
		temp_ptr = temp_ptr->lchild;
		for (int j = flag_start+1; j < ((NC_CFile*)cfile_array->data[i])->include_arr->elmcount; j++)
		{
			temp_ptr->rchild = nc_cfile_search(((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[j])->name);
			temp_ptr->rchild->parent = temp_ptr;
			temp_ptr = temp_ptr->rchild;
		}
	}
	/*for (int i = 0; i < cfile_array->elmcount; i++)
	{
		printf("%s FFF %s\n",((NC_CFile*)cfile_array->data[i])->comp_info->file_path, ((NC_CFile*)cfile_array->data[i])->comp_info->file_name);
		for (int j = 0; j < ((NC_CFile*)cfile_array->data[i])->include_arr->elmcount; j++)
		{
			printf("%s\n", ((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[j])->name);
		}
	}*/
	for (int i = 0; i < cfile_array->elmcount; i++)
	{
		//printf("%s\n", ((NC_CFile*)cfile_array->data[i])->comp_info->file_name);
		if ((((NC_CFile*)cfile_array->data[i])->parent == (NC_CFile*)cfile_array->data[i])&&!((((NC_CFile*)cfile_array->data[i])->lchild == (NC_CFile*)cfile_array->data[i])&& (((NC_CFile*)cfile_array->data[i])->rchild == (NC_CFile*)cfile_array->data[i])))
		{
			file_list= (NC_CFile*)cfile_array->data[i];
			//printf("=========ROOT=========\n"); 
		}
		if (((NC_CFile*)cfile_array->data[i])->rchild != NULL)
		{
			//printf("R: %s\n", ((NC_CFile*)cfile_array->data[i])->rchild->comp_info->file_name);
		}
		if (((NC_CFile*)cfile_array->data[i])->lchild != NULL)
		{
			//printf("L: %s\n", ((NC_CFile*)cfile_array->data[i])->lchild->comp_info->file_name);
		}
		if (((NC_CFile*)cfile_array->data[i])->parent != NULL)
		{
			//printf("P: %s\n", ((NC_CFile*)cfile_array->data[i])->parent->comp_info->file_name);
		}
	}
	/*printf("%d\n", cfile_array->elmcount);
	for (int i = 0; i < cfile_array->elmcount; i++)
	{
		printf("%s\n",((NC_CFile*)cfile_array->data[i])->comp_info->file_path);
	}*/
	/*nc_lex_init();
	nc_analyze_token();
	CToken *tk;
	int flag = 0;
	char *s = NULL,*next_path=NULL;*/
	/*__EH_DLIST_FOREACH(file_list->rchild->token_stream->stream, next, tk)
	{
		if (tk->token_type == CPKW_INCLUDE)
			flag = 1;
		else if (flag == 1 && tk->token_type == CTK_STR)
		{
			strcpy(next_path,file_path);
			strcat(next_path, eh_string_toasciistring(s, tk->token_value));
			printf("%s\n",next_path);
			flag = 0;
		}
		else if (flag == 1 && tk->token_type == CTK_IDENT)
		{
			next_path = eh_string_toasciistring(s, tk->token_value);
			printf("%s\n",next_path);
			flag = 0;
		}
	}*/
	return 0;
}

