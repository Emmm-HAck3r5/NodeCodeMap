/*
 * File: nc_csignparser.c
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-10 11:39:52
 * ------
 * Last Modified: 2017-12-10 18:07:58
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#include "nc_csignparser.h"

const char *nc_csign_char_set = "+-*/%=!<>&.|~^()[]{};,";
const char *nc_csign_first_char_set = "+-*/%=!<>&.|^";
_Bool nc_c_is_sign(char c)
{
	if ((strchr(nc_csign_char_set, c)) != NULL)
		return true;
	else
		return false;
}
void  nc_csign_parse(NC_File *fp, char c)
{
	CToken *tk;
	if ((strchr(nc_csign_first_char_set, c)) == NULL)
	{//µ¥×Ö·ûµÄ·ûºÅ
		switch (c)
		{
		case '~':
			tk = nc_ctoken_generate(COP_BNOT, NULL,current_lineno);
			nc_token_stream_add(tk);
			break;
		case '(':
			tk = nc_ctoken_generate(COP_LPA, NULL,current_lineno);
			nc_token_stream_add(tk);
			break;
			//todo
		}
		
	}
}