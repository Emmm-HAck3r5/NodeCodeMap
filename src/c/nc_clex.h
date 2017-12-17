/*
 * File: nc_clex.h
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-08 19:09:25
 * ------
 * Last Modified: 2017-12-10 18:06:18
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#ifndef NC_CLEX_H
#define NC_CLEX_H

#include "nc_ctoken.h"
#include "eh_dlist.h"
#include "nc_io.h"
#include "nc_csignparser.h"
#include "nc_cstruct.h"

#define NC_KEYWORD_COUNT 36
#define NC_PREKEYWORD_COUNT 13
extern NC_CFile *file_list;
extern EH_String *buffer;
extern u32 current_lineno;
typedef enum NC_CLexState
{
	CLEX_NORMAL,CLEX_IDENTIFIER,CLEX_SIGN,
	CLEX_STRING,CLEX_ANNOATAION,CLEX_SPACE,
	CLEX_NUM
}NC_CLexState;
void nc_token_stream_add(CToken *tk);
void nc_lex_init(void);
void nc_lex_open(NC_File *fp);
//void nc_lex_close(void);
int nc_is_in_set(char c, const char *set, int set_length);
int nc_is_identifier(char c);
int nc_is_keyword(EH_String *str);
int nc_is_prekeyword(EH_String *str);
void nc_refresh_buffer(u32 c);
char nc_parse_tmchar(char c);
int nc_analyze(NC_File *fp, u32 c);
void nc_analyze_token(NC_File *fp);
CToken* nc_lex_get_token(NC_CTokenStream *ts);
void nc_lex_unget_token(NC_CTokenStream *ts);
#endif