#include "nc_parser.h"
#include "nc_clex.h"

NC_CParseInfo* nc_cparse_info_init(void)
{
	NC_CParseInfo *info = (NC_CParseInfo*)malloc(sizeof(NC_CParseInfo));
	info->buffer = eh_string_init(64);
	info->type_modifier = 0;
	info->type_type = 0;
	info->token_type = CTK_NULL;
	info->dirty = 0;
	info->pos = -1;
	return info;
}
void nc_cparse_info_free(NC_CParseInfo *info)
{
	eh_string_free(info->buffer);
	free(info);
}

int nc_check_preprocess(CToken *token)
{
	switch (token->token_type)
	{
	case CPKW_DEFINE:
	case CPKW_DEFINED:
	case CPKW_ELIF:
	case CPKW_ELSE:
	case CPKW_ENDIF:
	case CPKW_ERROR:
	case CPKW_IF:
	case CPKW_IFDEF:
	case CPKW_IFNDEF:
	case CPKW_INCLUDE:
	case CPKW_LINE:
	case CPKW_PRAGMA:
	case CPKW_UNDEF:
		return 1;
	default:
		return 0;
	}
}

void nc_type_specifier_to_string(EH_String *dest, NC_CParseInfo *info)
{
	switch (info->token_type)
	{
	case CKW_VOID:
		eh_string_ccopy(dest, "void");
		break;
	case CKW_CHAR:
		eh_string_ccopy(dest, "char");
		break;
	case CKW_SHORT:
		eh_string_ccopy(dest, "short");
		break;
	case CKW_INT:
		eh_string_ccopy(dest, "int");
		break;
	case CKW_LONG:
		eh_string_ccopy(dest, "long");
		break;
	case CKW_FLOAT:
		eh_string_ccopy(dest, "float");
		break;
	case CKW_DOUBLE:
		eh_string_ccopy(dest, "double");
		break;
	case CKW_STRUCT:
		eh_string_ccopy(dest, "struct");
		eh_string_appendc(dest, ' ', 1);
		eh_string_cat(dest, info->buffer);
		break;
	case CKW_UNION:
		eh_string_ccopy(dest, "union");
		eh_string_appendc(dest, ' ', 1);
		eh_string_cat(dest, info->buffer);
		break;
	case CKW_ENUM:
		eh_string_ccopy(dest, "enum");
		eh_string_appendc(dest, ' ', 1);
		eh_string_cat(dest, info->buffer);
		break;
	case CTK_IDENT:
		eh_string_copy(dest, info->buffer);
	}
}
void nc_parse_translation_unit(NC_CFile *file)
{//parser���
	file->token_stream->file = file;
	CToken *token;
	while ((token = nc_lex_get_token(file->token_stream))->token_type != CTK_ENDSYMBOL)
	{
		//����Ԥ�������
		while (nc_check_preprocess(token) == 1)
		{
			while((token = nc_lex_get_token(file->token_stream))->token_type!=COP_SEMICOLON)
			;
			token = nc_lex_get_token(file->token_stream);
		}
		nc_lex_unget_token(file->token_stream);
		nc_parse_external_declaration(file);
	}
}
void nc_parse_external_declaration(NC_CFile *file)
{//parser�����߼�
	//�ڴ˲�������function��declaration
	//���н�����������ȫ���ݴ浽parseinfo
	NC_CParseInfo *info = nc_cparse_info_init();
	int ret_val;
	ret_val = nc_parse_declaration_specifier(file->token_stream, info);
	if (ret_val == NC_PARSE_TYPE)//���ν������������Ͷ��壬��������
		return;
	//�����Ǳ�������������������
	nc_parse_declarator(file->token_stream, info);
	return;
}
int nc_parse_declaration_specifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info)
{//���������޶����ɴ˽���������Ҳ�ڴ˽���
	int ret_val;
	int i, j, k;
	//todo ���ܴ��ڵĶ�ν���ѭ��
	nc_parse_storage_class_specifier(tk_stream, info);
	ret_val = nc_parse_type_specifier(tk_stream, info);
	nc_parse_type_qualifier(tk_stream, info);
	return ret_val;
}
int nc_parse_storage_class_specifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info)
{//�����洢����˵����
	CToken *token = nc_lex_get_token(tk_stream);
	switch (token->token_type)
	{
	case CKW_AUTO:
		if(info->pos<0)
			info->pos = token->file_pos;
		info->type_modifier |= NC_CTypeModifier_Auto;
		return NC_PARSE_SUCCESS;
	case CKW_REGISTER:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->type_modifier |= NC_CTypeModifier_Register;
		return NC_PARSE_SUCCESS;
	case CKW_STATIC:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->type_modifier |= NC_CTypeModifier_Static;
		return NC_PARSE_SUCCESS;
	case CKW_EXTERN:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->type_modifier |= NC_CTypeModifier_Extern;
		return NC_PARSE_SUCCESS;
	case CKW_TYPEDEF:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->type_modifier |= NC_CTypeModifier_Typedef;
		return NC_PARSE_SUCCESS;
	default:
		nc_lex_unget_token(tk_stream);
		//fprintf(stderr, "nc_parse_storage_class_specifier error\n");
		return NC_PARSE_FAILED;
	}
}
int nc_parse_type_specifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info)
{//��������˵����
	CToken *token;
	token = nc_lex_get_token(tk_stream);
	switch (token->token_type)
	{
	case CKW_VOID:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->token_type = CKW_VOID;
		return NC_PARSE_SUCCESS;
	case CKW_CHAR:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->token_type = CKW_CHAR;
		return NC_PARSE_SUCCESS;
	case CKW_SHORT:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->token_type = CKW_SHORT;
		return NC_PARSE_SUCCESS;
	case CKW_INT:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->token_type = CKW_INT;
		return NC_PARSE_SUCCESS;
	case CKW_LONG:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->token_type = CKW_LONG;
		return NC_PARSE_SUCCESS;
	case CKW_FLOAT:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->token_type = CKW_FLOAT;
		return NC_PARSE_SUCCESS;
	case CKW_DOUBLE:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->token_type = CKW_DOUBLE;
		return NC_PARSE_SUCCESS;
	case CKW_STRUCT:
	case CKW_UNION:
		if (info->pos<0)
			info->pos = token->file_pos;
		//�˴������������͵Ķ��壬�ڴ��жϷ���ֵ���������������Ҫ����NC_PARSE_TYPE
		nc_lex_unget_token(tk_stream);
		return nc_parse_struct_or_union_specifier(tk_stream, info);
	case CKW_ENUM:
		if (info->pos<0)
			info->pos = token->file_pos;
		//�˴������������͵Ķ��壬�ڴ��жϷ���ֵ���������������Ҫ����NC_PARSE_TYPE
		nc_lex_unget_token(tk_stream);
		return nc_parse_enum_specifier(tk_stream, info);
	case CTK_IDENT:
		if (info->pos<0)
			info->pos = token->file_pos;
		//ע���ʱparseinfo��buffer�Ѿ�����
		//todo ���typedef ֧��
		eh_string_copy(info->buffer, token->token_value);
		info->token_type = CTK_IDENT;
		info->dirty = 1;
		return NC_PARSE_SUCCESS;
	default:
		nc_lex_unget_token(tk_stream);
		return NC_PARSE_FAILED;
	}
}
int nc_parse_type_qualifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info)
{//���������޶���
	CToken *token = nc_lex_get_token(tk_stream);
	switch (token->token_type)
	{
	case CKW_CONST:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->type_modifier |= NC_CTypeModifier_Const;
		return NC_PARSE_SUCCESS;
	case CKW_VOLATILE:
		if (info->pos<0)
			info->pos = token->file_pos;
		info->type_modifier |= NC_CTypeModifier_Volatile;
		return NC_PARSE_SUCCESS;
	default:
		nc_lex_unget_token(tk_stream);
		return NC_PARSE_FAILED;
	}
}
int nc_parse_struct_or_union_specifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info)
{//����struct��union�����
	//��Ҫʹ��buffer��Ҫ��buffer�Ǹɾ���
	CToken *token;
	nc_parse_struct_or_union(tk_stream, info);
	token = nc_lex_get_token(tk_stream);
	if (token->token_type == CTK_IDENT)
	{//�����͵�����
		eh_string_copy(info->buffer, token->token_value);
		return NC_PARSE_SUCCESS;
	}
	else//��������������
		nc_lex_unget_token(tk_stream);
	token = nc_lex_get_token(tk_stream);
	if (token->token_type == COP_LCB)
	{//���͵Ķ���
		//�ڴ˴����������ͣ������parseinfo
		NC_CType *type = nc_ctype_init();
		type->type_modifier = info->type_modifier;
		type->type_type = info->type_type;
		type->comp_info->lineno = token->lineno;
		//�ļ�λ��
		type->comp_info->begin_pos = info->pos;
		eh_string_copy(type->type_name, info->buffer);
		//����type�ĳ�Ա����
		if (type->member == NULL)
		{
			type->member = (NC_CVariable*)malloc(sizeof(NC_CVariable));
			__EH_DLIST_INIT(type->member, next, prev);
		}
		//todo ��ȡ�ļ���������
		while (nc_parse_struct_declaration(tk_stream, type)!= NC_PARSE_SUCCESS);
		token = nc_lex_get_token(tk_stream);
		if (token->token_type != COP_RCB)
		{
			nc_lex_unget_token(tk_stream);
			fprintf(stderr, "nc_parse_struct_or_union_specifier error no rcb\n");
		}
		token = nc_lex_get_token(tk_stream);
		if (token->token_type != COP_SEMICOLON)
		{
			nc_lex_unget_token(tk_stream);
			fprintf(stderr, "nc_parse_struct_or_union_specifier error no sem\n");
		}
		//�ļ�λ��
		type->comp_info->end_pos = token->file_pos;
		//�����file��type��
		__EH_DLIST_ADD_TAIL(tk_stream->file->type_list, next, prev, type);
		return NC_PARSE_TYPE;
	}
	else
	{
		nc_lex_unget_token(tk_stream);
		fprintf(stderr, "nc_parse_struct_or_union_specifier error no lcb\n");
		return NC_PARSE_FAILED;
	}
}
void nc_parse_struct_or_union(NC_CTokenStream *tk_stream, NC_CParseInfo *info)
{//����struct��union�ؼ���
	CToken *token = nc_lex_get_token(tk_stream);
	switch (token->token_type)
	{
	case CKW_STRUCT:
		info->type_type |= NC_CType_Struct;
		return;
	case CKW_UNION:
		info->type_type |= NC_CType_Union;
		return;
	default:
		fprintf(stderr, "parse_struct_or_union error\n");
	}
}
int nc_parse_struct_declaration(NC_CTokenStream *tk_stream, NC_CType *type)
{//����struct��union��Ա
	int i;
	NC_CVariable *new_var = nc_cvariable_init();
	__EH_DLIST_INIT(new_var, next, prev);
	NC_CParseInfo *info = nc_cparse_info_init();
	while (nc_parse_specifier_qualifier(tk_stream, info) != NC_PARSE_FAILED)
		;
	//����parse��Ϣ
	new_var->var_type->type_type = info->type_type;
	new_var->var_type->type_modifier = info->type_modifier;
	//�Զ���������Ϣ
	nc_type_specifier_to_string(new_var->var_type->type_name, info);
	//����parseinfo
	nc_cparse_info_free(info);
	i = nc_parse_struct_declarator_list(tk_stream, new_var);
	//��new_var��������ӵ���Ա����
	__EH_DLIST_COMBINE_SPC(type->member, new_var, next, prev, next, prev);
	//;
	CToken *token = nc_lex_get_token(tk_stream);
	if (token->token_type != COP_SEMICOLON)
	{
		fprintf(stderr, "No semicolon nc_parse_struct_declaration\n");
		nc_lex_unget_token(tk_stream);
		return NC_PARSE_FAILED;
	}
	return i;
}
int nc_parse_specifier_qualifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info)
{//��������˵�������޶���
	int i, j;
	i=nc_parse_type_specifier(tk_stream, info);
	j=nc_parse_type_qualifier(tk_stream, info);
	if (i == NC_PARSE_FAILED && j == NC_PARSE_FAILED)
		return NC_PARSE_FAILED;
	else
		return NC_PARSE_SUCCESS;
}
int nc_parse_struct_declarator_list(NC_CTokenStream *tk_stream, NC_CVariable *var)
{//����ͬһ�����ڵ���,�ָ�����������֧�����顢ָ��
	CToken *token;
	NC_CVariable *new_var;
	while ((token = nc_lex_get_token(tk_stream))->token_type == CTK_IDENT)
	{
		//todo ʹ��parse_declarator��֧�ֶ�ָ�롢�����֧��
		new_var = nc_cvariable_init();
		//����ǳ�����޶���Ϣ
		new_var->var_type = var->var_type;
		new_var->type_modifier = var->type_modifier;
		new_var->comp_info = var->comp_info;

		eh_string_copy(new_var->var_name, token->token_value);
		__EH_DLIST_ADD_TAIL(var, next, prev, new_var);

		token = nc_lex_get_token(tk_stream);
		if (token->token_type == COP_COMMA)
			;
		else if (token->token_type == COP_SEMICOLON)
		{
			nc_lex_unget_token(tk_stream);
			return NC_PARSE_SUCCESS;
		}
		else
		{
			fprintf(stderr, "error nc_parse_struct_declarator_list\n");
			nc_lex_unget_token(tk_stream);
			return NC_PARSE_FAILED;
		}
	}
	nc_lex_unget_token(tk_stream);
	return NC_PARSE_FAILED;
}
int nc_parse_declarator(NC_CTokenStream *tk_stream, NC_CParseInfo *info)
{//����������
	//�����ͱ����������ڴ˽���
	//��������direct_declarator,ǰ������ȫ���ڴ˽�������
	nc_parse_pointer(tk_stream, info);
	CToken *token = nc_lex_get_token(tk_stream);
	//��ʶ��
	if (token->token_type != CTK_IDENT)
	{
		fprintf(stderr, "nc_parse_declarator error no identifier\n");
		nc_lex_unget_token(tk_stream);
		return NC_PARSE_FAILED;
	}
	else
	{
		//�����parseinfo�����Ѿ�����ģ��½�buffer��ȡ��ʶ����ֵ
		EH_String *tmp = eh_string_init(64);
		eh_string_copy(tmp, token->token_value);
		//�ж��Ǳ������Ǻ���
		token = nc_lex_get_token(tk_stream);
		if (token->token_type == COP_LPA)
		{//��������������������ֻ������������
			int i;
			//��ǰ�鿴
			for (i = 0;; i++)
			{
				token = nc_lex_get_token(tk_stream);
				if (token->token_type == COP_RPA)
					break;
			}
			token = nc_lex_get_token(tk_stream);
			if (token->token_type == COP_LCB)//�Ǻ�������
			{//����token��
				nc_lex_unget_token(tk_stream);
				for (; i >= 0; i--)
					nc_lex_unget_token(tk_stream);
				//�����º���
				NC_CFunction *func = nc_cfunction_init();
				//�ļ�λ��
				func->comp_info->begin_pos = info->pos;
				eh_string_copy(func->func_name, tmp);
				eh_string_free(tmp);
				//��parseinfo��ȡ����ֵ�����Ϣ
				//��parseinfo��token_typeת��func��ret_type->name
				func->func_ret_type->type_type = info->type_type;
				func->func_ret_type->type_modifier = info->type_modifier;
				nc_type_specifier_to_string(func->func_ret_type->type_name, info);
				//�����βα����
				//��ʱû��LPA
				nc_parse_parameter_list(tk_stream, func);
				//���Ժ�����
				token = nc_lex_get_token(tk_stream);
				if (token->token_type == COP_LCB)
				{
					int stack = 1;//ջ
					while (stack>0)
					{
						token = nc_lex_get_token(tk_stream);
						if (token->token_type == COP_RCB)
							stack--;
						else if (token->token_type == COP_LCB)
							stack++;
					}
					//�ļ�λ��
					func->comp_info->end_pos = token->file_pos;
				}
				else
				{
					nc_lex_unget_token(tk_stream);
					fprintf(stderr, "nc_parse_declarator no lcb\n");
					return NC_PARSE_FAILED;
				}
				//������ļ�function��
				__EH_DLIST_ADD_TAIL(tk_stream->file->function_list, next, prev, func);
				return NC_PARSE_FUNC;
			}
			else
			{//��������
				eh_string_free(tmp);
				if(token->token_type==COP_SEMICOLON)
					return NC_PARSE_SUCCESS;
				else
				{
					nc_lex_unget_token(tk_stream);
					fprintf(stderr, "nc_parse_declarator no ;\n");
					return NC_PARSE_FAILED;
				}
			}
		}
		else
		{//������ע���ʼ����������
			NC_CVariable *var = nc_cvariable_init();
			//�ļ�λ��
			var->comp_info->begin_pos = info->pos;

			eh_string_copy(var->var_name, tmp);
			eh_string_free(tmp);
			//��parseinfo��ȡ����������Ϣ
			var->var_type->type_modifier = info->type_modifier;
			var->var_type->type_type = info->type_type;
			//�Զ������͵Ļ�ȡ
			nc_type_specifier_to_string(var->var_type->type_name, info);
			if (token->token_type == COP_ASSIGN)
			{//������ʼ�����
				while ((token = nc_lex_get_token(tk_stream))->token_type != COP_SEMICOLON)
					;
				//��;�³��� :P
				//nc_lex_unget_token(tk_stream);
				//�ļ�λ��
				var->comp_info->end_pos = token->file_pos;
			}
			else if (token->token_type == COP_LBR)
			{//����
				var->var_type->type_type |= NC_CType_Array;
				while ((token = nc_lex_get_token(tk_stream))->token_type != COP_SEMICOLON)
					;
				//��;�³��� :P
				//nc_lex_unget_token(tk_stream);
				//�ļ�λ��
				var->comp_info->end_pos = token->file_pos;
			}
			else if (token->token_type == COP_SEMICOLON)
			{
				//�ļ�λ��
				var->comp_info->end_pos = token->file_pos;
			}
			else
			{
				nc_lex_unget_token(tk_stream);
			}
			//��var��ӵ�file��var��
			__EH_DLIST_ADD_TAIL(tk_stream->file->var_list, next, prev, var);
			return NC_PARSE_VAR;
		}
	}

}
int nc_parse_pointer(NC_CTokenStream *tk_stream, NC_CParseInfo *info)
{//����ָ��
	CToken *token = nc_lex_get_token(tk_stream);
	if (token->token_type == COP_STAR)
		info->type_type |= NC_CType_Pointer;
	else
	{
		nc_lex_unget_token(tk_stream);
		//fprintf(stderr, "nc_parse_pointer error\n");//debug��
		return NC_PARSE_FAILED;
	}
	while ((token = nc_lex_get_token(tk_stream))->token_type == COP_STAR)
	{
		;
	}
	nc_lex_unget_token(tk_stream);
	return NC_PARSE_SUCCESS;
}
int nc_parse_parameter_list(NC_CTokenStream *tk_stream, NC_CFunction *func)
{
	while (nc_parse_parameter_declaration(tk_stream, func) != NC_PARSE_FAILED)
		;
	return NC_PARSE_SUCCESS;
}
int nc_parse_parameter_declaration(NC_CTokenStream *tk_stream, NC_CFunction *func)
{
	CToken *token = nc_lex_get_token(tk_stream);
	if (token->token_type == COP_COMMA)
	{
		;
	}
	else if (token->token_type == COP_RPA)
	{
		return NC_PARSE_FAILED;
	}
	else
		nc_lex_unget_token(tk_stream);
	NC_CParseInfo *info = nc_cparse_info_init();
	NC_CVariable *var = nc_cvariable_init();
	//����������Ϣ
	nc_parse_declaration_specifier(tk_stream, info);
	nc_parse_pointer(tk_stream, info);
	//��ȡparseinfo��������Ϣ
	var->var_type->type_modifier = info->type_modifier;
	var->var_type->type_type = info->type_type;
	//�Զ������͵Ķ�ȡ
	nc_type_specifier_to_string(var->var_type->type_name, info);
	nc_parse_abstract_declarator(tk_stream, var);
	__EH_DLIST_ADD_TAIL(func->parameter, next, prev, var);
	nc_cparse_info_free(info);
	return NC_PARSE_SUCCESS;
}
int nc_parse_abstract_declarator(NC_CTokenStream *tk_stream, NC_CVariable *var)
{
	CToken *token = nc_lex_get_token(tk_stream);
	if (token->token_type == CTK_IDENT)
	{
		eh_string_copy(var->var_name, token->token_value);
		//����
		token = nc_lex_get_token(tk_stream);
		if (token->token_type == COP_RPA || token->token_type == COP_COMMA)
		{
			nc_lex_unget_token(tk_stream);
			return NC_PARSE_SUCCESS;
		}
		else if (token->token_type == COP_LBR)
		{
			var->var_type->type_type |= NC_CType_Array;
			//����ʣ�ಿ��
			while ((token = nc_lex_get_token(tk_stream))->token_type != COP_RBR)
				;
			return NC_PARSE_SUCCESS;
		}
		else
		{
			nc_lex_unget_token(tk_stream);
			fprintf(stderr, "nc_parse_abstract_declarator error\n");
			return NC_PARSE_FAILED;
		}
	}
	else
	{
		fprintf(stderr, "nc_parse_abstract_declarator error no ident\n");
		nc_lex_unget_token(tk_stream);
		return NC_PARSE_FAILED;
	}
}
int nc_parse_enum_specifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info)
{//enum�������
	CToken *token = nc_lex_get_token(tk_stream);
	if (token->token_type == CKW_ENUM)
	{
		info->type_type |= NC_CType_Enum;
	}
	token = nc_lex_get_token(tk_stream);
	if (token->token_type == CTK_IDENT)
	{//�����͵�����
		eh_string_copy(info->buffer, token->token_value);
		return NC_PARSE_SUCCESS;
	}
	else//��������������
		nc_lex_unget_token(tk_stream);
	token = nc_lex_get_token(tk_stream);
	if (token->token_type == COP_LCB)
	{//���͵Ķ���
	 //�ڴ˴����������ͣ������parseinfo
		NC_CType *type = nc_ctype_init();
		type->type_modifier = info->type_modifier;
		type->type_type = info->type_type;
		type->comp_info->lineno = token->lineno;
		//�ļ�λ��
		type->comp_info->begin_pos = info->pos;

		eh_string_copy(type->type_name, info->buffer);
		//����type�ĳ�Ա����
		if (type->member == NULL)
		{
			type->member = (NC_CVariable*)malloc(sizeof(NC_CVariable));
			__EH_DLIST_INIT(type->member, next, prev);
		}
		//todo ��ȡ�ļ���������
		while (nc_parse_enumerator_list(tk_stream, type) != NC_PARSE_SUCCESS);
		token = nc_lex_get_token(tk_stream);
		if (token->token_type != COP_RCB)
		{
			nc_lex_unget_token(tk_stream);
			fprintf(stderr, "nc_parse_enum_specifier error no rcb\n");
		}
		token = nc_lex_get_token(tk_stream);
		if (token->token_type != COP_SEMICOLON)
		{
			nc_lex_unget_token(tk_stream);
			fprintf(stderr, "nc_parse_enum_specifier error no sem\n");
		}
		type->comp_info->end_pos = token->file_pos;
		//�����file��type��
		__EH_DLIST_ADD_TAIL(tk_stream->file->type_list, next, prev, type);
		return NC_PARSE_TYPE;
	}
	else
	{
		nc_lex_unget_token(tk_stream);
		fprintf(stderr, "nc_parse_enum_specifier error no lcb\n");
		return NC_PARSE_FAILED;
	}
}
int nc_parse_enumerator_list(NC_CTokenStream *tk_stream, NC_CType *type)
{//enum�ڽ���
	CToken *token;
	NC_CVariable *new_var;
	while ((token = nc_lex_get_token(tk_stream))->token_type==CTK_IDENT)
	{
		new_var = nc_cvariable_init();
		eh_string_copy(new_var->var_name, token->token_value);
		//ÿ����Ա�������Ͷ���ENUM
		new_var->var_type->type_type = NC_CType_Enum;
		__EH_DLIST_ADD_TAIL(type->member, next, prev, new_var);
		token = nc_lex_get_token(tk_stream);
		if (token->token_type == COP_COMMA)
			;
		else if (token->token_type == COP_RCB)
		{
			return NC_PARSE_SUCCESS;
		}
		else
		{
			nc_lex_unget_token(tk_stream);
			fprintf(stderr, "nc_parse_enumerator_list error\n");
			return NC_PARSE_FAILED;
		}
	}
	return NC_PARSE_SUCCESS;
}

void nc_parse_get_decl(NC_CFile *file)
{
	NC_CVariable *var;
	NC_CType *type;
	NC_CFunction *func;
	__EH_DLIST_FOREACH(file->type_list, next, type)
	{
		eh_string_substr(type->comp_info->decl, file->pfile->file,
			type->comp_info->begin_pos, type->comp_info->end_pos);
	}
	__EH_DLIST_FOREACH(file->var_list, next, var)
	{
		eh_string_substr(var->comp_info->decl, file->pfile->file,
			var->comp_info->begin_pos, var->comp_info->end_pos);
	}
	__EH_DLIST_FOREACH(file->function_list, next, func)
	{
		eh_string_substr(func->comp_info->decl, file->pfile->file,
			func->comp_info->begin_pos, func->comp_info->end_pos);
	}
}
void nc_parse_test(NC_CFile *file)
{
	NC_CVariable *var;
	NC_CType *type;
	NC_CFunction *func;
	char *buf1 = (char*)malloc(sizeof(char) * 100);
	char *buf2 = (char*)malloc(sizeof(char) * 100);
	char *buf3 = (char*)malloc(sizeof(char) * 100);
	fprintf(stderr, "Type:\n");
	__EH_DLIST_FOREACH(file->type_list, next, type)
	{
		eh_string_toasciistring(buf1, type->type_name);
		eh_string_toasciistring(buf3, type->comp_info->decl);
		fprintf(stderr, "%s\n%s\n", buf1, buf3);
	}
	fprintf(stderr, "Var:\n");
	__EH_DLIST_FOREACH(file->var_list, next, var)
	{
		eh_string_toasciistring(buf1, var->var_type->type_name);
		eh_string_toasciistring(buf2, var->var_name);
		eh_string_toasciistring(buf3, var->comp_info->decl);
		fprintf(stderr, "%s %s\n%s\n", buf1, buf2, buf3);
	}
	fprintf(stderr, "Function:\n");
	__EH_DLIST_FOREACH(file->function_list, next, func)
	{
		eh_string_toasciistring(buf1, func->func_ret_type->type_name);
		eh_string_toasciistring(buf2, func->func_name);
		fprintf(stderr, "%s %s(", buf1, buf2);
		__EH_DLIST_FOREACH(func->parameter, next, var)
		{
			eh_string_toasciistring(buf1, var->var_type->type_name);
			eh_string_toasciistring(buf2, var->var_name);
			fprintf(stderr, "%s %s,", buf1, buf2);
		}
		fprintf(stderr, ")\n");
		buf3 = eh_string_toasciistring(buf3, func->comp_info->decl);
		fprintf(stderr, "%s\n", buf3);
	}
	fprintf(stderr, "Done!\n");
	free(buf1);
	free(buf2);
	free(buf3);
}