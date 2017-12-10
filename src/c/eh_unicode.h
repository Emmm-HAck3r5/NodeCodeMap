/*
 * File: eh_unicode.h
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-08 11:29:02
 * ------
 * Last Modified: 2017-12-08 14:15:42
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#ifndef EH_UNICODE_H
#define EH_UNICODE_H

#include "eh_typedef.h"
#include "eh_string.h"
#ifdef __cplusplus
extern "C" {
#endif
	void eh_utf8_to_utf32(EH_String *dest, u8 *utf8str);
#ifdef __cplusplus
}
#endif
#endif