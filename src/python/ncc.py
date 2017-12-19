# """
# PACK NC_CSTUCT.SO
# """
# import ctypes as ct
# print('NCC PACKING...')

# _path = ''
# ncc = ct.cdll.LoadLibrary(_path)

# class NC_CFile(ct.Structure):
#     _fields_ = [("parent", ct.POINTER(NC_CFile)),
#                 ("lchild", ct.POINTER(NC_CFile)),
#                 ("rchild", ct.POINTER(NC_CFile)),
#                 ("file_type", ct.c_int), # NC_CFileType (enum)
#                 ("comp_info", ct.POINTER(NC_CCompInfo)),
#                 ("token_stream", ct.POINTER(NC_CTokenStream)),
#                 ("function_list", ct.POINTER(NC_CFunction)),
#                 ("var_list", ct.POINTER(NC_CVariable)),
#                 ("macro_list", ct.POINTER(NC_CMacro)),
#                 ("type_list", ct.POINTER(NC_CType)),
#                 ("include_arr", ct.POINTER(EH_Array))]

# class NC_CTokenStream(ct.Structure):
#     _fields_ = [("stream", ct.POINTER(CToken)), 
#                 ("pos", ct.POINTER(CToken))]


# class NC_CCompInfo(ct.Structure):
#     _fields_ = [("file_data", ct.POINTER(EH_String)),
#                 ("file_path", ct.POINTER(ct.c_char)),
#                 ("file_name", ct.POINTER(ct.c_char)),
#                 ("decl", ct.POINTER(EH_String)),
#                 ("lineno", c_uint), # u32
#                 ("pos", c_uint)] # u32

# class NC_CType(ct.Structure):
#     _fields_ = [("next", ct.POINTER(NC_CType)),
#                 ("prev", ct.POINTER(NC_CType)),
#                 ("type_type", c_ubyte), # u8
#                 ("type_name", ct.POINTER(EH_String)),
#                 ("comp_info", ct.POINTER(NC_CCompInfo)),
#                 ("type_modifier", c_ubyte),# u8
#                 ("member", ct.POINTER(NC_CVariable))]

# class NC_CVariable(ct.Structure):
#     _fields_ = [("next", ct.POINTER(NC_CVariable)),
#                 ("prev", ct.POINTER(NC_CVariable)),
#                 ("var_type", NC_CType), # self-defined class valid ?
#                 ("var_name", ct.POINTER(EH_String)),
#                 ("comp_info", ct.POINTER(NC_CCompInfo)),
#                 ("type_modifier", c_ubyte)] # u8

# class NC_CFunction(ct.Structure):
#     _fields_ = [("next", ct.POINTER(NC_CFunction)),
#                 ("prev", ct.POINTER(NC_CFunction)),
#                 ("func_name", ct.POINTER(EH_String)),
#                 ("comp_info", ct.POINTER(NC_CCompInfo)),
#                 ("func_type", NC_CType), # self-defined class valid ?
#                 ("func_ret_type", c_ubyte),
#                 ("parameter", ct.POINTER(NC_CVariable)),
#                 ("func_body", ct.POINTER(EH_String))]

# class NC_CMacro(ct.Structure):
#     _fields_ = [("next", ct.POINTER()),
#                 ("prev", ct.POINTER()),
#                 ("macro_name", ct.POINTER()),
#                 ("comp_info", ct.POINTER()),
#                 ("parameter", ct.POINTER()),
#                 ("after_expand", ct.POINTER()),
#                 ("macrotype", c_int), # WHAT IS 'rplace'? 
#                 ("macro_name_tokens", ct.POINTER()),
#                 ("macro_afterexpand_tokens", ct.POINTER())]

# class NC_Include(ct.Structure):
#     _fields_ = [("name", ct.c_char_p),
#                 ("path", ct.c_char_p),
#                 ("type", ct.c_int),
#                 ("ptrs", ct.POINTER(EH_Array))]
# # NC_CFile* nc_cfile_init(NC_File *fp)
# file_init = ncc.nc_cfile_init
# file_init.argtypes = (ct.POINTER(NC_File))
# file_init.restype = ct.POINTER(NC_File)

# # NC_CCompInfo* nc_ccompinfo_init(void);
# compinfo_init = ncc.nc_ccompinfo_init
# compinfo_init.argtypes = ()
# compinfo_init.restype = ct.POINTER(NC_CCompInfo)

# # NC_CFile *nc_cfile_search(char *name);
# file_search = ncc.nc_cfile_search
# file_search.argtypes = (c_char_p)


# # NC_Include* nc_include_init();
# include_init = ncc.nc_include_init
# include_init.argtypes = ()
# include_init.restype = ct.POINTER(NC_Include)


# ct.POINTER()

# class (ct.Structure):
#     _fields_ = []

* parse(c_char_p)

