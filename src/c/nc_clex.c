/*
 * File: nc_clex.c
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-08 19:09:34
 * ------
 * Last Modified: 2017-12-10 18:06:08
 * Modified By: EasyAI ( easyai@outlook.com )
 */

#include "nc_clex.h"

CToken *token_stream;
EH_String *buffer;
u32 current_lineno;

const char nc_space[2] = { ' ','\t' };
void nc_token_stream_init(void)
{
	token_stream = nc_ctoken_generate(CTK_NULL, NULL, 0);
	__EH_DLIST_INIT(token_stream, next, prev);
}

void nc_token_stream_add(CToken *tk)
{
	current_token = tk;
	__EH_DLIST_ADD_TAIL(token_stream, next, prev, tk);
}
void nc_lex_open(void)
{
	if (token_stream)
	{
		CToken *tmp;
		CToken *tk=token_stream->next->next;
		do
		{
			tmp = tk->prev;
			__EH_DLIST_DELETE(token_stream, tk->prev, next, prev);
			nc_ctoken_destroy(tmp);
			tk = tk->next;
		} while (tk->prev != token_stream);
	}
	else
	{
		nc_token_stream_init();
		current_lineno = 0;
		current_token = NULL;
	}
}
void nc_lex_close(void)
{
	if (token_stream)
	{
		CToken *tmp;
		CToken *tk = token_stream->next->next;
		do
		{
			tmp = tk->prev;
			__EH_DLIST_DELETE(token_stream, tk->prev, next, prev);
			nc_ctoken_destroy(tmp);
			tk = tk->next;
		} while (tk->prev != token_stream);
		nc_ctoken_destroy(token_stream);
		token_stream = NULL;
	}
	current_lineno = 0;
	current_token = NULL;
}
_Bool nc_is_in_set(char c, const char *set, int set_length)
{
	int i;
	for (i = 0; i < set_length; i++)
		if (c == set[i])
			return true;
	return false;
}

_Bool nc_is_identifier(char c)
{
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z') || (c >= '0'&&c <= '9') || c == '_')
		return true;
	else
		return false;
}

void nc_refresh_buffer(u32 c)
{
	eh_string_clear(buffer);
	eh_string_appendc(buffer, c, 1);
}
char nc_parse_tmchar(char c)
{
	switch (c)
	{
	case '0':
		return '\0';
	case 'a':
		return '\a';
	case 'b':
		return '\b';
	case 't':
		return '\t';
	case 'n':
		return '\n';
	case 'v':
		return '\v';
	case 'f':
		return '\f';
	case 'r':
		return '\r';
	case '\"':
		return '\"';
	case '\'':
		return '\'';
	case '\\':
		return '\\';
	default:
		printf("Illegal character\n");
		return -1;
	}
}
_Bool nc_analyze_dep(NC_File *fp, u32 c)
{
	static int state = CLEX_NORMAL;
	_Bool move_cursor = true;
	u32 token_type;
	token_type = CTK_NULL;
	current_token = NULL;
	if (state = CLEX_NORMAL)
	{
		if (c == '#')
			state = CLEX_PREPROCESS;
		else if (c == '\n')
		{
			current_lineno++;
			move_cursor = true;
		}
		else if (c == '/')
		{
			current_lineno--;
			move_cursor = true;
		}
		else
			state = CLEX_SPACE;
		nc_refresh_buffer(c);
	}
	else if (state == CLEX_SPACE)
	{
		if (c=='\n'||c=='/'||c=='#')
		{
			state = CLEX_NORMAL;
			move_cursor = false;
		}
	}
	else if (state == CLEX_PREPROCESS)
	{
		//Ô¤´¦Àí
	}
	if (token_type != CTK_NULL)
	{
		current_token = nc_ctoken_generate(token_type, buffer, current_lineno);
		nc_token_stream_add(current_token);
	}
	return move_cursor;
}

void nc_analyze_token_dep(NC_File *fp)
{
	u32 c;
	nc_lex_open();
	while (!current_token)
	{
		c = nc_getch(fp);
		if (c == EOF)
		{
			current_token = nc_ctoken_generate(CTK_ENDSYMBOL, NULL, current_lineno);
			nc_token_stream_add(current_token);
			break;
		}
		while (!nc_analyze(fp, c))
		{
		}
	}
	nc_lex_close();
}

_Bool nc_analyze_pre(NC_File *fp, u32 c)
{

}

void nc_analyze_token_pre(NC_File *fp)
{

}

_Bool nc_analyze(NC_File *fp, u32 c)
{
	static int state = CLEX_NORMAL;
	static _Bool is_tmchar = false;
	_Bool move_cursor = true;
	u32 token_type;
	token_type = CTK_NULL;
	current_token = NULL;
	if (state = CLEX_NORMAL)
	{
		if (nc_is_identifier(c))
			state = CLEX_IDENTIFIER;
		else if (nc_c_is_sign(c))
		{
			state = CLEX_SIGN;
			move_cursor = false;
		}
		else if (c == '\"' || c == '\'')
		{
			state = CLEX_STRING;
			is_tmchar = false;
		}
		else if ((c >= '0'&&c <= '9') || c == '.')
		{
			state = CLEX_NUM;
		}
		else if (c == '/')
		{
			state = CLEX_ANNOATAION;
		}
		else if (nc_is_in_set(c, nc_space, 2))
			state = CLEX_SPACE;
		else if (c == '#')
			state = CLEX_PREPROCESS;
		else if (c == '\n')
		{
			current_lineno++;
			move_cursor = true;
		}
		else if (c == '/')
		{
			current_lineno--;
			move_cursor = true;
		}
		else
			printf("error\n");
		nc_refresh_buffer(c);
	}
	else if (state == CLEX_IDENTIFIER)
	{
		if (nc_is_identifier(c))
			eh_string_appendc(buffer, (u32)c, 1);
		else
		{
			token_type = CTK_IDENT;
			state = CLEX_NORMAL;
			move_cursor = false;
		}
	}
	else if (state == CLEX_SIGN)
	{
		if (c == '.')
		{
			char next = nc_getch(fp);
			if (next <= '9'&&next >= '0')
			{
				state = CLEX_NUM;
				move_cursor = false;
			}
			nc_ungetch(fp);
		}
		nc_csign_parse(fp, c);
	}
	else if (state = CLEX_STRING)
	{
		if (c == '\n')
			printf("Illegal string.\n");
		else if (is_tmchar)
		{
			char tmp = nc_parse_tmchar(c);
			eh_string_appendc(buffer, tmp, 1);
			is_tmchar = false;
		}
		else if (c == '\\')
			is_tmchar = true;
		else
		{
			eh_string_appendc(buffer, c, 1);
			if (c == buffer->value[0])
			{
				token_type = CTK_STR;
				state = CLEX_NORMAL;
			}
		}
	}
	else if (state == CLEX_NUM)
	{
		if (c == '.'||(c>='0'&&c<='9')||c=='e'||c=='E'||c=='x'||c=='X'||(c>='a'&&c<='f')||(c>='A'&&c<='F'))
		{
			eh_string_appendc(buffer, c, 1);
		}
		else
		{
			token_type = CTK_NUM;
			state = CLEX_NORMAL;
			move_cursor = false;
		}
	}
	else if (state == CLEX_ANNOATAION)
	{
		//½âÎö×¢ÊÍ
	}
	else if (state == CLEX_SPACE)
	{
		if (!nc_is_in_set(c, nc_space, 2))
		{
			state = CLEX_NORMAL;
			move_cursor = false;
		}
	}
	else if (state == CLEX_PREPROCESS)
	{
		//Ô¤´¦Àí
	}
	if (token_type != CTK_NULL)
	{
		current_token = nc_ctoken_generate(token_type, buffer, current_lineno);
		nc_token_stream_add(current_token);
	}
	return move_cursor;
}

void nc_analyze_token(NC_File *fp)
{
	u32 c;
	nc_lex_open();
	while (!current_token)
	{
		c = nc_getch(fp);
		if (c == EOF)
		{
			current_token = nc_ctoken_generate(CTK_ENDSYMBOL, NULL, current_lineno);
			nc_token_stream_add(current_token);
			break;
		}
		while (!nc_analyze(fp,c))
		{ }
	}
	nc_lex_close();
}