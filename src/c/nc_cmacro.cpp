#include"nc_cmacro.h"

//�ȴ�ת���ɺ�����ʽ
//int main(void)
//{
//	EH_Array *a = eh_array_init(16);
//	NC_File *file = nc_file_init();
//	nc_read_file(file, "E:\\Users\\wbl19\\Documents\\Tencent Files\\1276151929\\FileRecv\\NodeCodeC\\trail.h");
//	nc_lex_init();
//	nc_analyze_token(file);
//	CToken *tk;
//	char *s = NULL;
//
//	////////////////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//#define __EH_DLIST_ENDFIND(__list__,__end__,__ptr__) for(__ptr__=(__list__)->next;__ptr__!=__end__;__ptr__=__ptr__->next)
//
//	enum recog_state_enum { ininclude, inif, indefine, nostate }recog_state = nostate;
//
//	NC_CFile *cf = nc_cfile_init(file);
//
//	CToken *t1;
//	NC_CMacro *newmaco;
//	__EH_DLIST_INIT(cf->macro_list, next, prev);//��ʼ��macrolist ������γ��ļ������������������
//
//
//	__EH_DLIST_FOREACH(file_list->rchild->token_stream->stream, next, tk)
//	{
//		//printf("%s\n", eh_string_toasciistring(s, tk->token_value));
//		if (recog_state == nostate)
//		{
//			if (tk->token_type == CPKW_DEFINE)//#define
//			{
//				recog_state = indefine;
//				t1 = (CToken*)malloc(sizeof(CToken));
//				__EH_DLIST_INIT(t1, next, prev);
//			}
//			else if (tk->token_type == CPKW_INCLUDE)//#include
//			{
//				recog_state = ininclude;
//			}
//			else//��������������滻
//			{
//				NC_CMacro *pitpitmaco;
//				__EH_DLIST_FOREACH(cf->macro_list, next, pitpitmaco)//�����к�����
//				{
//					if (pitpitmaco->macrotype == rplace)
//					{
//						if ((tk->token_type == pitpitmaco->macro_name_tokens->token_type) && (tk->token_value == pitpitmaco->macro_name_tokens->token_value)) {//����ƥ�䣻Ŀǰ��������������
//							CToken *pitpittoken;
//							__EH_DLIST_FOREACH(pitpitmaco->macro_afterexpand_tokens, next, pitpittoken)
//							{
//								CToken *tokencopy = (CToken*)malloc(sizeof(CToken));
//								tokencopy->token_type = pitpittoken->token_type; tokencopy->token_value = pitpittoken->token_value; tokencopy->lineno = pitpittoken->lineno;
//								__EH_DLIST_INSERT(tk, next, prev, tokencopy);
//								__EH_DLIST_DELETE(file_list->rchild->token_stream->stream, tk, next, prev);//����һ������չ�������ļ�token��
//
//							}
//						}
//					}
//					else
//					{
//
//					}
//				}
//			}
//		}
//		else
//		{
//			if (tk->token_type == COP_SEMICOLON)//�ֺţ����������
//			{
//				recog_state = nostate;
//				newmaco = (NC_CMacro*)malloc(sizeof(NC_CMacro));
//				if (t1->next->next->token_type != COP_LPA)
//				{
//					newmaco->macro_name_tokens = t1->next;
//
//					CToken *temp = t1->next;
//					t1->next->next->prev = t1;
//					t1->next = t1->next->next;
//
//					newmaco->macro_afterexpand_tokens = t1;
//					newmaco->macrotype = rplace;
//				}
//				else//���������ţ����
//				{
//					CToken *pittoken;
//
//					__EH_DLIST_FOREACH(t1, next, pittoken)
//					{
//
//					}
//					newmaco->macrotype = function;
//				}
//				__EH_DLIST_ADD_TAIL(cf->macro_list, next, prev, newmaco);
//
//
//			}
//			else {
//				if (recog_state == indefine)
//				{
//					CToken *ttemp = (CToken*)malloc(sizeof(CToken));
//					ttemp->token_type = tk->token_type; ttemp->lineno = tk->lineno; ttemp->token_value = tk->token_value;
//					__EH_DLIST_ADD_TAIL(t1, next, prev, ttemp);
//
//				}
//				else if (recog_state == ininclude)
//				{
//
//				}
//				else
//				{
//
//				}
//			}
//		}
//
//
//	}
//
//	NC_CMacro *pitmaco;
//	CToken *pittoken;
//	__EH_DLIST_FOREACH(cf->macro_list, next, pitmaco)//��Щ������������ȷ��
//	{
//		if (pitmaco->macro_name_tokens->token_value != NULL) {
//			printf("%s\n", eh_string_toasciistring(s, pitmaco->macro_name_tokens->token_value));
//		}
//		else
//		{
//
//		}
//		__EH_DLIST_FOREACH(pitmaco->macro_afterexpand_tokens, next, pittoken) {
//			if (pittoken->token_value != NULL) {
//				printf("%s\n", eh_string_toasciistring(s, pittoken->token_value));
//			}
//		}
//	}
//
//
//
//
//
//
//	////////////////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////////////////
//	getchar();
//	getchar();
//	return 0;
//}