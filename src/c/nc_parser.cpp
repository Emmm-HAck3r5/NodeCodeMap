#include "nc_parser.h"
#include "nc_clex.h"

int nc_parse_type_specifier_helper(CToken *tk, NC_CType *type)
{
	switch (tk->token_type)
	{
	case CKW_CHAR:
		eh_string_ccopy(type->type_name, "char");
		return 0;
	case CKW_SHORT:
		eh_string_ccopy(type->type_name, "short");
		return 0;
	case CKW_INT:
		eh_string_ccopy(type->type_name, "int");
		return 0;
	case CKW_LONG:
		eh_string_ccopy(type->type_name, "long");
		return 0;
	case CKW_FLOAT:
		eh_string_ccopy(type->type_name, "float");
		return 0;
	case CKW_DOUBLE:
		eh_string_ccopy(type->type_name, "double");
		return 0;
	default:
		fprintf(stderr, "unsupported type specifier");
		return -1;
	}
}

void nc_parse_type_specifier(NC_CTokenStream *tkstream, NC_CType *type)
{
	CToken *token;
	token = nc_lex_get_token(tkstream);
	if (token->token_type == CKW_SIGNED)
	{
		type->type_type &= 0xEF;
		token = nc_lex_get_token(tkstream);
		nc_parse_type_specifier_helper(token, type);
	}
	else if (token->token_type == CKW_UNSIGNED)
	{
		type->type_type |= NC_CType_HBasic;
		token = nc_lex_get_token(tkstream);
		nc_parse_type_specifier_helper(token, type);
	}
}

void nc_parse_type_qualifier(NC_CTokenStream *tk_stream, void *var)
{
	CToken *token;
	while (1)
	{
		token = nc_lex_get_token(tk_stream);
		switch (token->token_type)
		{
		case CKW_AUTO:
			((NC_CType*)var)->type_modifier |= NC_CTypeModifier_Auto;
			break;
		case CKW_VOLATILE:
			((NC_CType*)var)->type_modifier |= NC_CTypeModifier_Volatile;
			break;
		case CKW_REGISTER:
			((NC_CType*)var)->type_modifier |= NC_CTypeModifier_Register;
			break;
		case CKW_EXTERN:
			((NC_CType*)var)->type_modifier |= NC_CTypeModifier_Extern;
			break;
		case CKW_STATIC:
			((NC_CType*)var)->type_modifier |= NC_CTypeModifier_Static;
			break;
		case CKW_CONST:
			((NC_CType*)var)->type_modifier |= NC_CTypeModifier_Const;
			break;
		default:
			nc_lex_unget_token(tk_stream);
			return;
		}
	}
}