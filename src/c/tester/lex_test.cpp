#include "../eh_string.h"
#include "../nc_io.h"
#include <stdio.h>
#include "../nc_clex.h"
int main(void)
{
	EH_Array *a = eh_array_init(16);
	NC_File *file = nc_file_init();
	nc_read_file(file, "D:\\Emmm_HAck3r5\\NodeCodeMap\\src\\c\\nc_io.h");
	nc_lex_init();
	nc_analyze_token(file);
	CToken *tk;
	char *s=NULL;
	__EH_DLIST_FOREACH(file_list->rchild->token_stream->stream, next, tk)
	{
		if (tk->token_type == CTK_IDENT)
			printf("IDENT ");
		else if (tk->token_type == CTK_STR)
			printf("STRING ");
		else if (tk->token_type == CTK_NUM)
			printf("NUMBER ");
		else if (tk->token_type <= COP_ELLIPSIS)
			printf("OP ");
		else if (tk->token_type <= CKW_INLINE)
			printf("KEYWORD ");
		else if (tk->token_type <= CPKW_LINE)
			printf("PREPROCESS ");
		else if (tk->token_type == CTK_ENDSYMBOL)
			printf("ENDSYMBOL ");
		printf("%s\n", eh_string_toasciistring(s, tk->token_value));
	}
	getchar();
	getchar();
	return 0;
}