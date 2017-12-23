/*
 * File: nc_ctoken.c
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-10 10:19:49
 * ------
 * Last Modified: 2017-12-10 18:07:40
 * Modified By: EasyAI ( easyai@outlook.com )
 */

#include "nc_ctoken.h"
#include <stdio.h>
CToken *current_token;
CToken* nc_ctoken_generate(CTokenType type, EH_String *val,u32 ln)
{
	CToken *tk = (CToken*)malloc(sizeof(CToken));
	if (val)
	{
		tk->token_value = eh_string_init(val->buffersize);
		eh_string_copy(tk->token_value, val);
	}
	else
		tk->token_value = NULL;
	tk->token_type = type;
	tk->lineno = ln;
	tk->file_pos = 0;
	return tk;
}

void nc_ctoken_destroy(CToken *tk)
{
	eh_string_free(tk->token_value);
	free(tk);
}

void nc_print_token_type(u32 token_type)
{
	switch (token_type)
	{
	case 0: fprintf(stderr, "COP_PLUS"); break;
	case 1: fprintf(stderr, "COP_MINUS"); break;
	case 2: fprintf(stderr, "COP_STAR"); break;
	case 3: fprintf(stderr, "COP_DIVIDE"); break;
	case 4: fprintf(stderr, "COP_MOD"); break;
	case 5: fprintf(stderr, "COP_PP"); break;
	case 6: fprintf(stderr, "COP_MM"); break;
	case 7: fprintf(stderr, "COP_PE"); break;
	case 8: fprintf(stderr, "COP_ME"); break;
	case 9: fprintf(stderr, "COP_TE"); break;
	case 10: fprintf(stderr, "COP_DE"); break;
	case 11: fprintf(stderr, "COP_MODE"); break;
	case 12: fprintf(stderr, "COP_EQ"); break;
	case 13: fprintf(stderr, "COP_NEQ"); break;
	case 14: fprintf(stderr, "COP_LT"); break;
	case 15: fprintf(stderr, "COP_LTEQ"); break;
	case 16: fprintf(stderr, "COP_RT"); break;
	case 17: fprintf(stderr, "COP_RTEQ"); break;
	case 18: fprintf(stderr, "COP_ASSIGN"); break;
	case 19: fprintf(stderr, "COP_DOT"); break;
	case 20: fprintf(stderr, "COP_AND"); break;
	case 21: fprintf(stderr, "COP_OR"); break;
	case 22: fprintf(stderr, "COP_NOT"); break;
	case 23: fprintf(stderr, "COP_BAND"); break;
	case 24: fprintf(stderr, "COP_BOR"); break;
	case 25: fprintf(stderr, "COP_BNOT"); break;
	case 26: fprintf(stderr, "COP_BXOR"); break;
	case 27: fprintf(stderr, "COP_SHL"); break;
	case 28: fprintf(stderr, "COP_SHR"); break;
	case 29: fprintf(stderr, "COP_BANDE"); break;
	case 30: fprintf(stderr, "COP_BORE"); break;
	case 31: fprintf(stderr, "COP_BXORE"); break;
	case 32: fprintf(stderr, "COP_SHLE"); break;
	case 33: fprintf(stderr, "COP_SHRE"); break;
	case 34: fprintf(stderr, "COP_LPA"); break;
	case 35: fprintf(stderr, "COP_RPA"); break;
	case 36: fprintf(stderr, "COP_LBR"); break;
	case 37: fprintf(stderr, "COP_RBR"); break;
	case 38: fprintf(stderr, "COP_LCB"); break;
	case 39: fprintf(stderr, "COP_RCB"); break;
	case 40: fprintf(stderr, "COP_SEMICOLON"); break;
	case 41: fprintf(stderr, "COP_COMMA"); break;
	case 42: fprintf(stderr, "COP_ELLIPSIS"); break;
	case 43: fprintf(stderr, "COP_POINTERTO"); break;
	case 44: fprintf(stderr, "COP_COLON"); break;
	case 45: fprintf(stderr, "CKW_AUTO"); break;
	case 46: fprintf(stderr, "CKW_SHORT"); break;
	case 47: fprintf(stderr, "CKW_INT"); break;
	case 48: fprintf(stderr, "CKW_LONG"); break;
	case 49: fprintf(stderr, "CKW_FLOAT"); break;
	case 50: fprintf(stderr, "CKW_DOUBLE"); break;
	case 51: fprintf(stderr, "CKW_CHAR"); break;
	case 52: fprintf(stderr, "CKW_STRUCT"); break;
	case 53: fprintf(stderr, "CKW_UNION"); break;
	case 54: fprintf(stderr, "CKW_ENUM"); break;
	case 55: fprintf(stderr, "CKW_TYPEDEF"); break;
	case 56: fprintf(stderr, "CKW_CONST"); break;
	case 57: fprintf(stderr, "CKW_UNSIGNED"); break;
	case 58: fprintf(stderr, "CKW_SIGNED"); break;
	case 59: fprintf(stderr, "CKW_EXTERN"); break;
	case 60: fprintf(stderr, "CKW_REGISTER"); break;
	case 61: fprintf(stderr, "CKW_STATIC"); break;
	case 62: fprintf(stderr, "CKW_VOLATILE"); break;
	case 63: fprintf(stderr, "CKW_VOID"); break;
	case 64: fprintf(stderr, "CKW_IF"); break;
	case 65: fprintf(stderr, "CKW_ELSE"); break;
	case 66: fprintf(stderr, "CKW_SWITCH"); break;
	case 67: fprintf(stderr, "CKW_FOR"); break;
	case 68: fprintf(stderr, "CKW_DO"); break;
	case 69: fprintf(stderr, "CKW_WHILE"); break;
	case 70: fprintf(stderr, "CKW_GOTO"); break;
	case 71: fprintf(stderr, "CKW_CONTINUE"); break;
	case 72: fprintf(stderr, "CKW_BREAK"); break;
	case 73: fprintf(stderr, "CKW_DEFAULT"); break;
	case 74: fprintf(stderr, "CKW_SIZEOF"); break;
	case 75: fprintf(stderr, "CKW_RETURN"); break;
	case 76: fprintf(stderr, "CKW_TYPEOF"); break;
	case 77: fprintf(stderr, "CKW_STDCALL"); break;
	case 78: fprintf(stderr, "CKW_CDECL"); break;
	case 79: fprintf(stderr, "CKW_FASTCALL"); break;
	case 80: fprintf(stderr, "CKW_INLINE"); break;
	case 81: fprintf(stderr, "CPKW_DEFINE"); break;
	case 82: fprintf(stderr, "CPKW_UNDEF"); break;
	case 83: fprintf(stderr, "CPKW_INCLUDE"); break;
	case 84: fprintf(stderr, "CPKW_IF"); break;
	case 85: fprintf(stderr, "CPKW_IFDEF"); break;
	case 86: fprintf(stderr, "CPKW_IFNDEF"); break;
	case 87: fprintf(stderr, "CPKW_ELIF"); break;
	case 88: fprintf(stderr, "CPKW_ENDIF"); break;
	case 89: fprintf(stderr, "CPKW_ELSE"); break;
	case 90: fprintf(stderr, "CPKW_ERROR"); break;
	case 91: fprintf(stderr, "CPKW_DEFINED"); break;
	case 92: fprintf(stderr, "CPKW_PRAGMA"); break;
	case 93: fprintf(stderr, "CPKW_LINE"); break;
	case 94: fprintf(stderr, "CTK_STR"); break;
	case 95: fprintf(stderr, "CTK_NUM"); break;
	case 96: fprintf(stderr, "CTK_ANNO"); break;
	case 97: fprintf(stderr, "CTK_IDENT"); break;
	case 98: fprintf(stderr, "CTK_NULL"); break;
	case 99: fprintf(stderr, "CTK_ENDSYMBOL"); break;
	}
}