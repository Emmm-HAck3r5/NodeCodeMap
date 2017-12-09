/*
 * File: eh_stack.h
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-08 14:11:27
 * ------
 * Last Modified: 2017-12-08 14:16:26
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#ifndef EH_STACK_H
#define EH_STACK_H

/* 栈顶不存储数据 */
#define __EH_STACK_INIT(__stack__,__mnext__) {\
	(__stack__)->__mnext__=NULL;\
}
#define __EH_STACK_EMPTY(__stack__,__mnext__) ((__stack__)->__mnext__==NULL?true:false)
#define __EH_STACK_POP(__stack__,__mnext__,__ret__) {\
	if(__EH_STACK_EMPTY(__stack__,__mnext__))\
		__ret__=NULL;\
	else\
	{\
	__ret__=(__stack__)->__mnext__;\
	(__stack__)->__mnext__=__ret__->__mnext__;\
	__ret__->__mnext__=NULL;\
	}\
}
#define __EH_STACK_PUSH(__stack__,__mnext__,__node__) {\
	__node__->__mnext__=(__stack__)->__mnext__;\
	(__stack__)->__mnext__=__node__;\
}
#define __EH_STACK_GET_TOP(__stack__,__mnext__,__ret__) {\
	__ret__=(__stack__)->__mnext__;\
}
#define __EH_STACK_FOREACH(__stack__,__mnext__,__ptr__) for(__ptr__=(__stack__)->__mnext__;__ptr__;__ptr__=__ptr__->__mnext__)
#endif