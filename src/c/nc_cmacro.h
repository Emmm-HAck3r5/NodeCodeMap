#ifndef NC_CMACRO_H
#define NC_CMACRO_H

#include"nc_io.h"
#include<stdio.h>
#include"nc_clex.h"
#include"nc_cstruct.h"
#include"eh_string.h"

#define __EH_DLIST_ENDFIND(__list__,__end__,__ptr__) for(__ptr__=(__list__)->next;__ptr__!=__end__;__ptr__=__ptr__->next)


#endif
