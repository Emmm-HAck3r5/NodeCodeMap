#ifndef NC_PARSER_H
#define NC_PARSER_H

#include "nc_cstruct.h"

void nc_parse_external_declaration(NC_CFile *file);
void nc_parse_type_specifier(NC_CTokenStream *tkstream, NC_CType *type);
void nc_parse_type_qualifier(NC_CTokenStream *tk_stream, void *var);//传入的是NC_CType或NC_CVariable

#endif