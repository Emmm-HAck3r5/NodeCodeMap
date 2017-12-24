#ifndef DOCK_H
#define DOCK_H

#include "nc_cstruct.h"
#include "nc_filedep.h"
#include "nc_parser.h"
#include "eh_string.h"
#include "nc_io.h"
#include "nc_clex.h"


extern "C" __declspec(dllexport) NC_CFile *nc_dock_generate(const char *dir);

#endif
