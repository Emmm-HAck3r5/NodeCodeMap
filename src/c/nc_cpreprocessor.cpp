/*
* File: nc_cpreprocessor.c
* Author: EasyAI
* Email: easyai@outlook.com
* Created Date: 2017-12-08 22:15:30
* ------
* Last Modified: 2017-12-10 18:08:16
* Modified By: EasyAI ( easyai@outlook.com )
*/
#include "nc_cstruct.h"
#include "nc_cpreprocessor.h"


void macroprocess(NC_CFile *cf)
{
	enum recog_state_enum { ininclude, inif, indefine, nostate }recog_state = nostate;

	CToken *t1, *tk;
	NC_CMacro *newmaco;

	__EH_DLIST_INIT(cf->macro_list, next, prev);//��ʼ��macrolist ������γ��ļ������������������
	bool dellasttk = false; CToken *lastk;

	__EH_DLIST_FOREACH(cf->token_stream->stream, next, tk)
	{
		if (dellasttk == true)
		{
			__EH_DLIST_DELETE(cf->token_stream->stream, lastk, next, prev);
			dellasttk = false;
		}
		if (recog_state == nostate)
		{
			if (tk->token_type == CPKW_DEFINE)//#define
			{
				recog_state = indefine;
				t1 = (CToken*)malloc(sizeof(CToken));
				__EH_DLIST_INIT(t1, next, prev);
			}
			else if (tk->token_type == CPKW_INCLUDE)//#include
			{
				recog_state = ininclude;
			}
			else//��������������滻
			{
				NC_CMacro *pitpitmaco;
				__EH_DLIST_FOREACH(cf->macro_list, next, pitpitmaco)//�����к�����
				{
					if (pitpitmaco->macrotype == rplace)
					{
						if ((tk->token_type == pitpitmaco->macro_name_tokens->token_type) && (*(tk->token_value->value) == *(pitpitmaco->macro_name_tokens->token_value->value))) {//����ƥ�䣻Ŀǰ��������pƥ��
							CToken *pitpittoken;
							CToken *pnowtoken = tk;
							__EH_DLIST_FOREACH(pitpitmaco->macro_afterexpand_tokens, next, pitpittoken)
							{
								CToken *tokencopy = (CToken*)malloc(sizeof(CToken));
								tokencopy->token_type = pitpittoken->token_type; tokencopy->token_value = pitpittoken->token_value; tokencopy->lineno = pitpittoken->lineno;
								__EH_DLIST_INSERT(pnowtoken, next, prev, tokencopy);
								pnowtoken = pnowtoken->next;
								//����һ������չ�������ļ�token��
							}
							dellasttk = true; lastk = tk;
						}
					}
					else
					{

					}
				}
			}
		}
		else
		{
			if (tk->token_type == COP_SEMICOLON)//�ֺţ����������
			{
				recog_state = nostate;
				newmaco = (NC_CMacro*)malloc(sizeof(NC_CMacro));
				if (t1->next->next->token_type != COP_LPA)
				{
					newmaco->macro_name_tokens = t1->next;

					CToken *temp = t1->next;
					t1->next->next->prev = t1;
					t1->next = t1->next->next;

					newmaco->macro_afterexpand_tokens = t1;
					newmaco->macrotype = rplace;
				}
				else//���������ţ����
				{
					CToken *pittoken;

					__EH_DLIST_FOREACH(t1, next, pittoken)
					{

					}
					newmaco->macrotype = function;
				}
				__EH_DLIST_ADD_TAIL(cf->macro_list, next, prev, newmaco);


			}
			else {
				if (recog_state == indefine)
				{
					CToken *ttemp = (CToken*)malloc(sizeof(CToken));
					ttemp->token_type = tk->token_type; ttemp->lineno = tk->lineno; ttemp->token_value = tk->token_value;
					__EH_DLIST_ADD_TAIL(t1, next, prev, ttemp);

				}
				else if (recog_state == ininclude)
				{

				}
				else
				{

				}
			}
		}


	}
}