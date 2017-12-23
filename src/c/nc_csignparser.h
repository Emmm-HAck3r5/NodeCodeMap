/*
 * File: nc_csignparser.h
 * Author: EasyAI
 * Email: easyai@outlook.com
 * Created Date: 2017-12-10 11:40:19
 * ------
 * Last Modified: 2017-12-10 18:07:51
 * Modified By: EasyAI ( easyai@outlook.com )
 */
#ifndef NC_CSIGNPARSER_H
#define NC_CSIGNPARSER_H

#include "nc_ctoken.h"
#include "nc_io.h"
#include "eh_typedef.h"
#include "nc_clex.h"
int nc_c_is_sign(char c);
void  nc_csign_parse(NC_File *fp,char c);
#endif