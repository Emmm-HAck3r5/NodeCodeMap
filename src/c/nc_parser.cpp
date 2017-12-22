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
	return info;
}
void nc_cparse_info_free(NC_CParseInfo *info)
{
	eh_string_free(info->buffer);
	free(info);
}
void nc_parse_translation_unit(NC_CFile *file)
{//parser���
	CToken *token;
	while ((token = nc_lex_get_token(file->token_stream))->token_type != CTK_ENDSYMBOL)
	{
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
	CToken *token;
	nc_lex_get_token(tk_stream);
	switch (token->token_type)
	{
	case CKW_AUTO:
		info->type_modifier |= NC_CTypeModifier_Auto;
		return NC_PARSE_SUCCESS;
	case CKW_REGISTER:
		info->type_modifier |= NC_CTypeModifier_Register;
		return NC_PARSE_SUCCESS;
	case CKW_STATIC:
		info->type_modifier |= NC_CTypeModifier_Static;
		return NC_PARSE_SUCCESS;
	case CKW_EXTERN:
		info->type_modifier |= NC_CTypeModifier_Extern;
		return NC_PARSE_SUCCESS;
	case CKW_TYPEDEF:
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
		info->token_type = CKW_VOID;
		return NC_PARSE_SUCCESS;
	case CKW_CHAR:
		info->token_type = CKW_CHAR;
		return NC_PARSE_SUCCESS;
	case CKW_SHORT:
		info->token_type = CKW_SHORT;
		return NC_PARSE_SUCCESS;
	case CKW_INT:
		info->token_type = CKW_INT;
		return NC_PARSE_SUCCESS;
	case CKW_LONG:
		info->token_type = CKW_LONG;
		return NC_PARSE_SUCCESS;
	case CKW_FLOAT:
		info->token_type = CKW_FLOAT;
		return NC_PARSE_SUCCESS;
	case CKW_DOUBLE:
		info->token_type = CKW_DOUBLE;
		return NC_PARSE_SUCCESS;
	case CKW_STRUCT:
	case CKW_UNION:
		//�˴������������͵Ķ��壬�ڴ��жϷ���ֵ���������������Ҫ����NC_PARSE_TYPE
		nc_lex_unget_token(tk_stream);
		return nc_parse_struct_or_union_specifier(tk_stream, info);
	case CKW_ENUM:
		//�˴������������͵Ķ��壬�ڴ��жϷ���ֵ���������������Ҫ����NC_PARSE_TYPE
		nc_lex_unget_token(tk_stream);
		return nc_parse_enum_specifier(tk_stream, info);
	case CTK_IDENT:
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
		info->type_modifier |= NC_CTypeModifier_Const;
		return NC_PARSE_SUCCESS;
	case CKW_VOLATILE:
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
		//todo �����file��type��
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
	//todo �Զ���������Ϣ
	eh_string_copy(new_var->var_type->type_name, info->buffer);
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
				eh_string_copy(func->func_name, tmp);
				eh_string_free(tmp);
				//��parseinfo��ȡ����ֵ�����Ϣ
				//todo ��parseinfo��token_typeת��func��ret_type->name
				func->func_ret_type->type_type = info->type_type;
				func->func_ret_type->type_modifier = info->type_modifier;
				
				//�����βα����
				//��ʱû��LPA
				nc_parse_parameter_list(tk_stream, func);

				//todo ������ļ�function��
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
			eh_string_copy(var->var_name, tmp);
			eh_string_free(tmp);
			//��parseinfo��ȡ����������Ϣ
			var->var_type->type_modifier = info->type_modifier;
			var->var_type->type_type = info->type_type;
			//todo �Զ������͵Ļ�ȡ
			
			token = nc_lex_get_token(tk_stream);
			if (token->token_type == COP_ASSIGN)
			{//������ʼ�����
				while ((token = nc_lex_get_token(tk_stream))->token_type != COP_SEMICOLON)
					;
				//��;�³��� :P
				nc_lex_unget_token(tk_stream);
			}
			else if (token->token_type == COP_LBR)
			{//����
				var->var_type->type_type |= NC_CType_Array;
				while ((token = nc_lex_get_token(tk_stream))->token_type != COP_SEMICOLON)
					;
				//��;�³��� :P
				nc_lex_unget_token(tk_stream);
			}
			else
			{
				nc_lex_unget_token(tk_stream);
			}
			//todo ��var��ӵ�file��var��
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
		fprintf(stderr, "nc_parse_pointer error\n");//debug��
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
	//todo �Զ������͵Ķ�ȡ

	nc_parse_abstract_declarator(tk_stream, var);
	__EH_DLIST_ADD_TAIL(func->parameter, next, prev, var);
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
		//todo �����file��type��
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