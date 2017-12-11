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
const char* nc_ckeyword[NC_KEYWORD_COUNT] = {
	"auto","short","int","long",
	"float","double","char","struct",
	"union","enum","typedef","const",
	"unsigned","signed","extern","register",
	"static","volatile","void","if",
	"else","switch","for","do",
	"while","goto","continue","break",
	"default","sizeof","return","typeof",
	"__stdcall","__cdecl","__fastcall","inline"
};
const char* nc_cprekeyword[NC_PREKEYWORD_COUNT] = {
	"#define","#undef","#include",
	"#if","#ifdef","#ifndef",
	"#elif","#endif","#else",
	"#error","defined","#pragma",
	"#line"
};
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
	if (buffer == NULL)
	{
		buffer = eh_string_init(256);
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
	free(buffer);
}
int nc_is_in_set(char c, const char *set, int set_length)
{
	int i;
	for (i = 0; i < set_length; i++)
		if (c == set[i])
			return true;
	return false;
}

int nc_is_identifier(char c)
{
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z') || (c >= '0'&&c <= '9') || c == '_')
		return true;
	else
		return false;
}

int nc_is_keyword(EH_String *str)
{
	int i = 0;
	for (i = 0; i < NC_KEYWORD_COUNT; i++)
	{
		if (eh_string_ascii_compare(str, nc_ckeyword[i]))
			return CKW_AUTO + i;
	}
	return 0;
}

int nc_is_prekeyword(EH_String *str)
{
	int i = 0;
	for (i = 0; i < NC_PREKEYWORD_COUNT; i++)
	{
		if (eh_string_ascii_compare(str, nc_cprekeyword[i]))
			return CPKW_DEFINE + i;
	}
	return 0;
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

int nc_analyze(NC_File *fp, u32 c)
{
	static int state = CLEX_NORMAL;
	static int is_tmchar = false;
	static int is_preprocess = false;
	static int is_line_link = false;
	static int anno_type = -1;
	int move_cursor = true;
	u32 token_type;
	token_type = CTK_NULL;
	current_token = NULL;
	if (state == CLEX_NORMAL)
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
		{
			state = CLEX_IDENTIFIER;
			move_cursor = true;
			is_preprocess = true;
		}
		else if (c == '\n')
		{
			if (is_line_link == false)
			{
				if (is_preprocess == true)//所有的预处理指令不再以\n为结束，统一以;为结束
				{
					current_token = nc_ctoken_generate(COP_SEMICOLON, NULL, current_lineno);
					nc_token_stream_add(current_token);
					is_preprocess = false;
				}
				current_lineno++;
			}
			move_cursor = true;
		}
		else if (c == '\\')
		{
			is_line_link = true;
			move_cursor = true;
		}
		else if (c == NC_FILE_EOF)
		{
			token_type = CTK_ENDSYMBOL;
		}
		else
			printf("error\n");
		nc_refresh_buffer(c);
	}
	else if (state == CLEX_IDENTIFIER)
	{
		if (nc_is_identifier(c)||c=='#')
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
		char next = nc_getch(fp);
		if (c == '.')
		{
			if (next <= '9'&&next >= '0')
			{
				state = CLEX_NUM;
				move_cursor = false;
			}
		}
		else if (c == '/')
		{
			if (next == '/' || next == '*')
			{
				state = CLEX_ANNOATAION;
				move_cursor = true;
			}
		}
		nc_ungetch(fp);
		if(state==CLEX_SIGN)
		{
			nc_csign_parse(fp, c);
			token_type = CTK_NULL;
			state = CLEX_NORMAL;
			move_cursor = true;
		}
	}
	else if (state == CLEX_STRING)
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
		if (anno_type == -1)
		{
			if (c == '/')
			{
				anno_type = 0;
			}
			else if (c == '*')
			{
				anno_type = 1;
			}
			eh_string_appendc(buffer, c, 1);
		}
		else if (anno_type == 0)
		{
			eh_string_appendc(buffer, c, 1);
			if (c == '\n')
			{
				state = CLEX_NORMAL;
				token_type = CTK_ANNO;
				move_cursor = false;
			}
		}
		else if (anno_type == 1)
		{
			eh_string_appendc(buffer, c, 1);
			if (c == '/')
			{
				if (buffer->value[buffer->length - 2] == '*')
				{
					state = CLEX_NORMAL;
					token_type = CTK_ANNO;
					move_cursor = true;
				}
			}
		}
	}
	else if (state == CLEX_SPACE)
	{
		if (!nc_is_in_set(c, nc_space, 2))
		{
			state = CLEX_NORMAL;
			move_cursor = false;
		}
	}
	if (token_type != CTK_NULL)
	{
		int keyword_name = 0;
		if (token_type == CTK_IDENT&&(keyword_name=nc_is_keyword(buffer))!=0)
		{
			current_token = nc_ctoken_generate((CTokenType)keyword_name, NULL, current_lineno);
			nc_token_stream_add(current_token);
		}
		else if (token_type == CTK_IDENT && (keyword_name = nc_is_prekeyword(buffer)) != 0)
		{
			current_token = nc_ctoken_generate((CTokenType)keyword_name, NULL, current_lineno);
			nc_token_stream_add(current_token);
		}
		else
		{
			current_token = nc_ctoken_generate((CTokenType)token_type, buffer, current_lineno);
			nc_token_stream_add(current_token);
		}
		if (token_type == CTK_ANNO)
			anno_type = -1;//复位
	}
	return move_cursor;
}

void nc_analyze_token(NC_File *fp)
{
	u32 c;
	while (token_stream->prev->token_type!=CTK_ENDSYMBOL)
	{
		c = nc_getch(fp);
		while (!nc_analyze(fp,c))
		{ }
	}
}