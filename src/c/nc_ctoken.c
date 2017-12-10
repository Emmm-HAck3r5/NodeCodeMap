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
	return tk;
}

void nc_ctoken_destroy(CToken *tk)
{
	eh_string_free(tk->token_value);
	free(tk);
}