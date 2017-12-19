#include "nc_filedep.h"

char *nc_filedep_getname(char *path)
{
	char *name = (char*)malloc(300);
	memset(name, 0, 300);
	strcpy(name,path);
	while (strchr(name, '\\') != 0)
	{
		name++;
	}
	return name;
}

NC_CFile *nc_filedep_search(char *name)//查找文件
{
	//printf("searching: \"%s\"\n",name);
	int i;
	for (i = 0; i < cfile_array->elmcount; i++)
	{
		//printf("checking: \"%s\"\nresult: ", ((NC_CFile*)cfile_array->data[i])->comp_info->file_name);
		if (strcmp(nc_filedep_getname(((NC_CFile*)cfile_array->data[i])->comp_info->file_path), name) == 0)
		{
			//printf("match: %s\n\n", ((NC_CFile*)cfile_array->data[i])->comp_info->file_name);
			return (NC_CFile*)cfile_array->data[i];
		}
		//printf("unmatch\n");
	}
	//printf("failed\n\n");
	return NULL;
}

//处理后filelist指向根文件
//filelist结构：每个节点右孩子与其地位平等，左孩子是其依赖的文件
//若父亲是自己，说明为根节点
//若某一孩子为自己，说明没有这个孩子
//暂不支持出现不同目录拥有同名文件
void *nc_file_dep_generate(const char *dir_path)
{
	EH_Array *file_array_path = eh_array_init(300);//储存所有文件路径的数组
	//EH_Array *cfile_array = eh_array_init(300);
	nc_get_cfiles(file_array_path, dir_path);//读取目录下所有文件的路径
	NC_File *file=nc_file_init();//用于读文件的临时变量
	CToken *temp_tk;//用于遍历token的临时变量
	bool include_flag = false;//用于判断是否为头文件相对路径
	char *temp_str=NULL;//临时数组
	struct NC_Include *icl_info;//存储include内容的临时变量
	nc_lex_init();
	int i;
	int j;
	for (i = 0; i < file_array_path->elmcount; i++)//对于每个文件
	{
		file = nc_file_init();//给他申请一个空间
		//icl_info = nc_include_init();
		nc_read_file(file, (char*)file_array_path->data[i]);//向空间中读入文件
		//printf("%s\n",(char*)file_array_path->data[i]);
		nc_analyze_token(file);//分析文件，并储存入CFile数组
		__EH_DLIST_FOREACH(((NC_CFile*)cfile_array->data[i])->token_stream->stream, next, temp_tk)//分析
		{
			//printf("%s\n",eh_string_toasciistring(temp_string,temp_tk->token_value));
			if (temp_tk->token_type == CPKW_INCLUDE)//出现#include
				include_flag = true;
			else if (include_flag && temp_tk->token_type == CTK_STR)//包含自定义头文件
			{
				temp_str = (char*)malloc(1000);//临时数组
				memset(temp_str, 0, 1000);
				icl_info = nc_include_init();
				include_flag = false;//处理完成，取消#include的出现状态
				icl_info->type = NC_CST;//自定义头文件
				temp_str = eh_string_toasciistring(temp_str, temp_tk->token_value);//获取带引号的头文件相对路径
				temp_str++;//删除第一个引号
				if (strchr(temp_str, '\"') == NULL) continue;
				strncpy(icl_info->path, temp_str, (int)(strchr(temp_str, '\"') - temp_str));//删掉末尾的引号
				strcpy(icl_info->name, icl_info->path);
				while (strchr(icl_info->name, '\\') != NULL)
					icl_info->name++;//获取文件名,非相对路径
				eh_array_append(((NC_CFile*)cfile_array->data[i])->include_arr, icl_info);//给对应文件加上该头文件
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
	for (i = 0; i < cfile_array->elmcount; i++)//初始化CFile树结构
	{
		((NC_CFile*)cfile_array->data[i])->lchild = (NC_CFile*)cfile_array->data[i];
		((NC_CFile*)cfile_array->data[i])->rchild = (NC_CFile*)cfile_array->data[i];
		((NC_CFile*)cfile_array->data[i])->parent = (NC_CFile*)cfile_array->data[i];
	}
	NC_CFile *temp_ptr;
	bool is_1st_c=true;
	for (i = 0; i < cfile_array->elmcount; i++)
		if (((NC_CFile*)cfile_array->data[i])->file_type == NC_CFile_C)
		{
			if (is_1st_c)
			{
				file_list = (NC_CFile*)cfile_array->data[i];
				is_1st_c = false;
				temp_ptr = file_list;
			}
			else
			{
				temp_ptr->rchild = (NC_CFile*)cfile_array->data[i];
				temp_ptr->rchild->parent = temp_ptr;
				temp_ptr = temp_ptr->rchild;
			}
		}
	int flag_start = 0;
	for (i = 0; i < cfile_array->elmcount; i++)
	{
		flag_start = 0;
		temp_ptr = (NC_CFile*)cfile_array->data[i];
		if ((((NC_CFile*)cfile_array->data[i])->include_arr->elmcount) == 0)
		{
			//printf("ERROR 1 NULL");
			continue;
		}
		/*
		while ((nc_cfile_search(((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[flag_start])->name) == NULL)&&flag_start<((NC_CFile*)cfile_array->data[i])->include_arr->elmcount-2)
			flag_start++;
		if (flag_start<)
		printf("STEP 1");
		printf("%d\n", (NC_Include*)(NC_CFile*)cfile_array);
		printf("%d\n", (NC_Include*)(NC_CFile*)cfile_array->data[i]);
		printf("%d\n", (NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr);
		printf("%d\n", ((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[flag_start]));
		*/
		temp_ptr->lchild = nc_filedep_search(((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[flag_start])->name);//左孩子为第一个头文件
		if (nc_filedep_search(((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[flag_start])->name) == NULL)
			continue;
		temp_ptr->lchild->parent = temp_ptr;
		temp_ptr = temp_ptr->lchild;
		for (int j = flag_start+1; j < ((NC_CFile*)cfile_array->data[i])->include_arr->elmcount; j++)//剩下的头文件都是左孩子的右孩子链
		{
			temp_ptr->rchild = nc_filedep_search(((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[j])->name);
			if (temp_ptr->rchild == NULL)
				continue;
			temp_ptr->rchild->parent = temp_ptr;
			temp_ptr = temp_ptr->rchild;
		}
	}
	/*
	for (int i = 0; i < cfile_array->elmcount; i++)
	{
		printf("%s FFF %s\n",((NC_CFile*)cfile_array->data[i])->comp_info->file_path, ((NC_CFile*)cfile_array->data[i])->comp_info->file_name);
		for (int j = 0; j < ((NC_CFile*)cfile_array->data[i])->include_arr->elmcount; j++)
		{
			printf("%s\n", ((NC_Include*)((NC_CFile*)cfile_array->data[i])->include_arr->data[j])->name);
		}
	}
	*/
	/*for (i = 0; i < cfile_array->elmcount; i++)
	{
		printf("%s\n", ((NC_CFile*)cfile_array->data[i])->comp_info->file_path);
		if (((NC_CFile*)cfile_array->data[i])->rchild != NULL)
		{
			printf("R: %s\n", ((NC_CFile*)cfile_array->data[i])->rchild->comp_info->file_path);
		}
		if (((NC_CFile*)cfile_array->data[i])->lchild != NULL)
		{
			printf("L: %s\n", ((NC_CFile*)cfile_array->data[i])->lchild->comp_info->file_path);
		}
		if (((NC_CFile*)cfile_array->data[i])->parent != NULL)
		{
			printf("P: %s\n", ((NC_CFile*)cfile_array->data[i])->parent->comp_info->file_path);
		}
	}*/
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

