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
int nc_c_is_sign(char c)
{
	if ((strchr(nc_csign_char_set, c)) != NULL&&c!='\0')
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
		case ')':
			tk = nc_ctoken_generate(COP_RPA, NULL, current_lineno);
			nc_token_stream_add(tk);
			break;
		case '[':
			tk = nc_ctoken_generate(COP_LBR, NULL, current_lineno);
			nc_token_stream_add(tk);
			break;
		case ']':
			tk = nc_ctoken_generate(COP_RBR, NULL, current_lineno);
			nc_token_stream_add(tk);
			break;
		case '{':
			tk = nc_ctoken_generate(COP_LCB, NULL, current_lineno);
			nc_token_stream_add(tk);
			break;
		case '}':
			tk = nc_ctoken_generate(COP_RCB, NULL, current_lineno);
			tk->file_pos = fp->file_ptr - 1;
			nc_token_stream_add(tk);
			break;
		case ';':
			tk = nc_ctoken_generate(COP_SEMICOLON, NULL, current_lineno);
			tk->file_pos = fp->file_ptr - 1;
			nc_token_stream_add(tk);
			break;
		case ',':
			tk = nc_ctoken_generate(COP_COMMA, NULL, current_lineno);
			nc_token_stream_add(tk);
			break;
		case ':':
			tk = nc_ctoken_generate(COP_COLON, NULL, current_lineno);
			nc_token_stream_add(tk);
			break;
		}
	}
	else
	{//¶à×Ö·ûµÄ·ûºÅ
		char next1, next2;
		switch (c)
		{
		case '+':
			next1 = nc_getch(fp);
			if (next1 == '+')
			{
				tk = nc_ctoken_generate(COP_PP, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else if (next1 == '=')
			{
				tk = nc_ctoken_generate(COP_PE, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else
			{
				tk = nc_ctoken_generate(COP_PLUS, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		case '-':
			next1 = nc_getch(fp);
			if (next1 == '-')
			{
				tk = nc_ctoken_generate(COP_MM, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else if (next1 == '=')
			{
				tk = nc_ctoken_generate(COP_ME, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else if (next1 == '>')
			{
				tk = nc_ctoken_generate(COP_POINTERTO, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else
			{
				tk = nc_ctoken_generate(COP_MINUS, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		case '*':
			next1 = nc_getch(fp);
			if (next1 == '=')
			{
				tk = nc_ctoken_generate(COP_TE, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else
			{
				tk = nc_ctoken_generate(COP_STAR, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		case '/':
			next1 = nc_getch(fp);
			if (next1 == '=')
			{
				tk = nc_ctoken_generate(COP_DE, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else
			{
				tk = nc_ctoken_generate(COP_DIVIDE, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		case '%':
			next1 = nc_getch(fp);
			if (next1 == '=')
			{
				tk = nc_ctoken_generate(COP_MODE, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else
			{
				tk = nc_ctoken_generate(COP_MOD, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		case '=':
			next1 = nc_getch(fp);
			if (next1 == '=')
			{
				tk = nc_ctoken_generate(COP_EQ, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else
			{
				tk = nc_ctoken_generate(COP_ASSIGN, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		case '!':
			next1 = nc_getch(fp);
			if (next1 == '=')
			{
				tk = nc_ctoken_generate(COP_NEQ, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else
			{
				tk = nc_ctoken_generate(COP_NOT, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		case '<':
			next1 = nc_getch(fp);
			if (next1 == '=')
			{
				tk = nc_ctoken_generate(COP_LTEQ, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else if(next1=='<')
			{
				next2 = nc_getch(fp);
				if (next2 == '=')
				{
					tk = nc_ctoken_generate(COP_SHLE, NULL, current_lineno);
					nc_token_stream_add(tk);
				}
				else
				{
					tk = nc_ctoken_generate(COP_SHL, NULL, current_lineno);
					nc_token_stream_add(tk);
					nc_ungetch(fp);
				}
			}
			else
			{
				tk = nc_ctoken_generate(COP_LT, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		case '>':
			next1 = nc_getch(fp);
			if (next1 == '=')
			{
				tk = nc_ctoken_generate(COP_RTEQ, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else if (next1 == '>')
			{
				next2 = nc_getch(fp);
				if (next2 == '=')
				{
					tk = nc_ctoken_generate(COP_SHRE, NULL, current_lineno);
					nc_token_stream_add(tk);
				}
				else
				{
					tk = nc_ctoken_generate(COP_SHR, NULL, current_lineno);
					nc_token_stream_add(tk);
					nc_ungetch(fp);
				}
			}
			else
			{
				tk = nc_ctoken_generate(COP_RT, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		case '.':
			next1 = nc_getch(fp);
			if (next1 == '.')
			{
				next2 = nc_getch(fp);
				if (next2 == '.')
				{
					tk = nc_ctoken_generate(COP_ELLIPSIS, NULL, current_lineno);
					nc_token_stream_add(tk);
				}
			}
			else
			{
				tk = nc_ctoken_generate(COP_DOT, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		case '&':
			next1 = nc_getch(fp);
			if (next1 == '=')
			{
				tk = nc_ctoken_generate(COP_BANDE, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else if (next1 == '&')
			{
				tk = nc_ctoken_generate(COP_AND, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else
			{
				tk = nc_ctoken_generate(COP_BAND, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		case '|':
			next1 = nc_getch(fp);
			if (next1 == '=')
			{
				tk = nc_ctoken_generate(COP_BORE, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else if (next1 == '|')
			{
				tk = nc_ctoken_generate(COP_OR, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else
			{
				tk = nc_ctoken_generate(COP_BOR, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		case '^':
			next1 = nc_getch(fp);
			if (next1 == '=')
			{
				tk = nc_ctoken_generate(COP_BXORE, NULL, current_lineno);
				nc_token_stream_add(tk);
			}
			else
			{
				tk = nc_ctoken_generate(COP_BXOR, NULL, current_lineno);
				nc_token_stream_add(tk);
				nc_ungetch(fp);
			}
			break;
		}
	}
}