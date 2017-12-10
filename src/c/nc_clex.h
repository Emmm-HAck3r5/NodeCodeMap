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
extern CToken *token_stream;
extern EH_String *buffer;
extern u32 current_lineno;
typedef enum NC_CLexState
{
	CLEX_NORMAL,CLEX_IDENTIFIER,CLEX_SIGN,
	CLEX_STRING,CLEX_ANNOATAION,CLEX_SPACE,
	CLEX_NUM,CLEX_PREPROCESS
}NC_CLexState;
void nc_token_stream_init(void);
void nc_token_stream_add(CToken *tk);
void nc_lex_open(void);
void nc_lex_close(void);
_Bool nc_is_in_set(char c, const char *set, int set_length);
_Bool nc_is_identifier(char c);
void nc_refresh_buffer(u32 c);
char nc_parse_tmchar(char c);
_Bool nc_analyze_dep(NC_File *fp, u32 c);
void nc_analyze_token_dep(NC_File *fp);
_Bool nc_analyze_pre(NC_File *fp, u32 c);
void nc_analyze_token_pre(NC_File *fp);
_Bool nc_analyze(NC_File *fp, u32 c);
void nc_analyze_token(NC_File *fp);
#endif