#ifndef NC_PARSER_H
#define NC_PARSER_H

#include "nc_cstruct.h"

#define NC_PARSE_FUNC 1
#define NC_PARSE_VAR 2
#define NC_PARSE_TYPE 3
#define NC_PARSE_SUCCESS 0
#define NC_PARSE_FAILED -1
typedef struct NC_CParseInfo
{
	EH_String *buffer;
	u8 type_modifier;
	u8 type_type;
	u32 token_type;
	u8 dirty;
}NC_CParseInfo;
//注意对解析信息的复写

NC_CParseInfo* nc_cparse_info_init(void);
void nc_cparse_info_free(NC_CParseInfo* info);
//parser入口
void nc_parse_translation_unit(NC_CFile *file);
//parser核心逻辑
void nc_parse_external_declaration(NC_CFile *file);
int nc_parse_declaration_specifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info);
int nc_parse_storage_class_specifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info);
int nc_parse_type_specifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info);
int nc_parse_type_qualifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info);
void nc_parse_struct_or_union(NC_CTokenStream *tk_stream, NC_CParseInfo *info);
int nc_parse_struct_or_union_specifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info);
int nc_parse_struct_declaration(NC_CTokenStream *tk_stream, NC_CType *type);
int nc_parse_specifier_qualifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info);
int nc_parse_struct_declarator_list(NC_CTokenStream *tk_stream, NC_CVariable *var);
int nc_parse_declarator(NC_CTokenStream *tk_stream, NC_CParseInfo *info);
int nc_parse_pointer(NC_CTokenStream *tk_stream, NC_CParseInfo *info);
int nc_parse_parameter_list(NC_CTokenStream *tk_stream, NC_CFunction *func);
int nc_parse_parameter_declaration(NC_CTokenStream *tk_stream, NC_CFunction *func);
int nc_parse_abstract_declarator(NC_CTokenStream *tk_stream, NC_CVariable *var);
void nc_parse_direct_abstract_declarator(NC_CTokenStream *tk_stream, NC_CFunction *func);
int nc_parse_enum_specifier(NC_CTokenStream *tk_stream, NC_CParseInfo *info);
int nc_parse_enumerator_list(NC_CTokenStream *tk_stream, NC_CType *type);
void nc_declaration(NC_CTokenStream *tk_stream, void *var);

#endif