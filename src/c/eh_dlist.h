/*
 * File: eh_dlist.h
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-08 14:00:43
 * ------
 * Last Modified: 2017-12-08 14:15:33
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#ifndef EH_DLIST_H
#define EH_DLIST_H

/* 链表头不存储数据*/
#define __EH_DLIST_INIT(__list__,__mnext__,__mprev__) {\
	(__list__)->__mnext__=__list__;\
	(__list__)->__mprev__=__list__;\
}
#define __EH_DLIST_EMPTY(__list__,__mnext__,__mprev__) (((__list__)->__mprev__==__list__&&(__list__)->__mnext__==__list__)?true:false)
#define __EH_DLIST_INSERT(__pos__,__mnext__,__mprev__,__node__) {\
	(__pos__)->__mnext__->__mprev__=__node__;\
	(__node__)->__mnext__=(__pos__)->__mnext__;\
	(__node__)->__mprev__=__pos__;\
	(__pos__)->__mnext__=__node__;\
}
#define __EH_DLIST_DELETE(__list__,__pos__,__mnext__,__mprev__) {\
	if(!__EH_DLIST_EMPTY(__list__,__mnext__,__mprev__))\
	{\
		(__pos__)->__mnext__->__mprev__=(__pos__)->__mprev__;\
		(__pos__)->__mprev__->__mnext__=(__pos__)->__mnext__;\
		(__pos__)->__mnext__=NULL;\
		(__pos__)->__mprev__=NULL;\
	}\
}
#define __EH_DLIST_ADD_TAIL(__list__,__mnext__,__mprev__,__node__) __EH_DLIST_INSERT(__list__,__mprev__,__mnext__,__node__)
#define __EH_DLIST_ADD_HEAD(__list__,__mnext__,__mprev__,__node__) __EH_DLIST_INSERT(__list__,__mnext__,__mprev__,__node__)
#define __EH_DLIST_FOREACH(__list__,__mnext__,__ptr__) for(__ptr__=(__list__)->__mnext__;__ptr__!=__list__;__ptr__=__ptr__->__mnext__)
#define __EH_DLIST_FIND_NODE(__list__,__mnext__,__ret__,__key__,__cmpfunc__) {\
	__EH_DLIST_FOREACH(__list__,__mnext__,__ret__)\
		if(!__cmpfunc__(__key__,__ret__)) {\
			break;\
	if(__ret__==__list__) __ret__=NULL;\
}
#endif