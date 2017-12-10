/*
 * File: nc_ctoken.h
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-10 10:19:59
 * ------
 * Last Modified: 2017-12-10 18:07:31
 * Modified By: EasyAI ( easyai@outlook.com )
 */

#ifndef NC_CTOKEN_H
#define NC_CTOKEN_H

#include "eh_typedef.h"
#include "eh_array.h"
#include "eh_string.h"

typedef enum CTokenType
{
	//运算符
	COP_PLUS,	// +
	COP_MINUS,	// -
	COP_STAR,	// *
	COP_DIVIDE,	// /
	COP_MOD,	// %
	COP_PP,		// ++
	COP_MM,		// --
	COP_PE,		// +=
	COP_ME,		// -=
	COP_TE,		// *=
	COP_DE,		// /=
	COP_MODE,	// %=
	COP_EQ,		// ==
	COP_NEQ,	// !=
	COP_LT,		// <
	COP_LTEQ,	// <=
	COP_RT,		// >
	COP_RTEQ,	// >=
	COP_ASSIGN,	// =
	COP_DOT,	// .
	COP_AND,	// &&
	COP_OR,		// ||
	COP_NOT,	// !
	COP_BAND,	// &
	COP_BOR,	// |
	COP_BNOT,	// ~
	COP_BXOR,	// ^
	COP_SHL,	// <<
	COP_SHR,	// >>
	COP_BANDE,	// &=
	COP_BORE,	// |=
	COP_BXORE,	// ^=
	COP_SHLE,	// <<=
	COP_SHRE,	// >>=
	COP_LPA,	// (
	COP_RPA,	// )
	COP_LBR,	// [
	COP_RBR,	// ]
	COP_LCB,	// {
	COP_RCB,	// }
	COP_SEMICOLON, // ;
	COP_COMMA,	// ,
	COP_ELLIPSIS, // ...
	//关键字
	CKW_AUTO,
	CKW_SHORT,
	CKW_INT,
	CKW_LONG,
	CKW_FLOAT,
	CKW_DOUBLE,
	CKW_CHAR,
	CKW_STRUCT,
	CKW_UNION,
	CKW_ENUM,
	CKW_TYPEDEF,
	CKW_CONST,
	CKW_UNSIGNED,
	CKW_SIGNED,
	CKW_EXTERN,
	CKW_REGISTER,
	CKW_STATIC,
	CKW_VOLATILE,
	CKW_VOID,
	CKW_IF,
	CKW_ELSE,
	CKW_SWITCH,
	CKW_FOR,
	CKW_DO,
	CKW_WHILE,
	CKW_GOTO,
	CKW_CONTINUE,
	CKW_BREAK,
	CKW_DEFAULT,
	CKW_SIZEOF,
	CKW_RETURN,
	CKW_TYPEOF, //gcc typeof
	CKW_STDCALL,
	CKW_CDECL,
	CKW_FASTCALL,
	CKW_INLINE,
	//预处理关键字
	CPKW_DEFINE,
	CPKW_UNDEF,
	CPKW_INCLUDE,
	CPKW_IF,
	CPKW_IFDEF,
	CPKW_IFNDEF,
	CPKW_ELIF,
	CPKW_ENDIF,
	CPKW_ELSE,
	CPKW_ERROR,
	CPKW_DEFINED,
	CPKW_PRAGMA,
	CPKW_LINE,
	//字面量（所有字面常量全当作字符串处理）
	CTK_STR,
	CTK_NUM,
	CTK_ANNO,
	//标识符
	CTK_IDENT,
	//预留状态
	CTK_NULL,
	CTK_ENDSYMBOL
}CTokenType;

typedef struct CToken
{
	struct CToken *next, *prev;
	CTokenType token_type;
	EH_String *token_value;
	u32 lineno;
}CToken;

//#ifdef __cplusplus
//extern "C" {
//#endif
	CToken* nc_ctoken_generate(CTokenType type, EH_String *val,u32 ln);
	void nc_ctoken_destroy(CToken *tk);

	extern CToken *current_token;
//#ifdef __cplusplus
//}
//#endif
#endif