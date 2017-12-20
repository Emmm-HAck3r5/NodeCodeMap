"""
PACK NC_CSTUCT.SO
"""
import ctypes as ct
print('NCC PACKING...')

_path = ''
ncc = ct.cdll.LoadLibrary(_path)

# parse
parse = ncc.parse
parse.argtypes = (c_wchar_p)
parse.restype = ct.POINTER(NC_File)

class NC_CFile(ct.Structure):
    pass
class NC_CTokenStream(ct.Structure):
    pass
class NC_CCompInfo(ct.Structure):
    pass
class NC_CType(ct.Structure):
    pass
class NC_CVariable(ct.Structure):
    pass
class NC_CFunction(ct.Structure):
    pass
class NC_CMacro(ct.Structure):
    pass

NC_CFile._fields_ = [("parent", ct.POINTER(NC_CFile)),
                    ("lchild", ct.POINTER(NC_CFile)),
                    ("rchild", ct.POINTER(NC_CFile)),
                    ("file_type", ct.c_uint16), # NC_CFileType (enum)
                    ("comp_info", ct.POINTER(NC_CCompInfo)),
                    ("token_stream", ct.POINTER(NC_CTokenStream)),
                    ("function_list", ct.POINTER(NC_CFunction)),
                    ("var_list", ct.POINTER(NC_CVariable)),
                    ("macro_list", ct.POINTER(NC_CMacro)),
                    ("type_list", ct.POINTER(NC_CType)),
                    ("include_arr", ct.POINTER(EH_Array))] # ? EH_Array is not defined

NC_CTokenStream._fields_ = [("stream", ct.POINTER(CToken)), 
                            ("pos", ct.POINTER(CToken))]

NC_CCompInfo._fields_ = [("file_data", ct.POINTER(EH_String)), # ? EH_String is not defined
                        ("file_path", ct.POINTER(ct.c_char)),
                        ("file_name", ct.POINTER(ct.c_char)),
                        ("decl", ct.POINTER(EH_String)), # ? EH_String is not defined
                        ("lineno", c_uint32), # u32
                        ("pos", c_uint32)] # u32

NC_CType._fields_ = [("next", ct.POINTER(NC_CType)),
                    ("prev", ct.POINTER(NC_CType)),
                    ("type_type", c_uint8), # u8
                    ("name", ct.POINTER(EH_String)), # ? EH_String is not defined
                    ("comp_info", ct.POINTER(NC_CCompInfo)),
                    ("type_modifier", c_uint8),# u8
                    ("member", ct.POINTER(NC_CVariable))]

NC_CVariable._fields_ = [("next", ct.POINTER(NC_CVariable)),
                        ("prev", ct.POINTER(NC_CVariable)),
                        ("var_type", NC_CType), # self-defined class valid ?
                        ("name", ct.POINTER(EH_String)), # ? EH_String is not defined
                        ("comp_info", ct.POINTER(NC_CCompInfo)),
                        ("type_modifier", c_uint8)] # u8

NC_CFunction._fields_ = [("next", ct.POINTER(NC_CFunction)),
                        ("prev", ct.POINTER(NC_CFunction)),
                        ("name", ct.POINTER(EH_String)), # ? EH_String is not defined
                        ("comp_info", ct.POINTER(NC_CCompInfo)),
                        ("func_type", c_uint8), # self-defined class valid ?
                        ("func_ret_type", NC_CType),
                        ("parameter", ct.POINTER(NC_CVariable)),
                        ("func_body", ct.POINTER(EH_String))] # ? EH_String is not defined

NC_CMacro._fields_ = [("next", ct.POINTER(NC_CMacro)),
                    ("prev", ct.POINTER(NC_CMacro)),
                    ("name", ct.POINTER(EH_String)), # ? EH_String is not defined
                    ("comp_info", ct.POINTER(NC_CCompInfo)),
                    ("parameter", ct.POINTER(EH_String)), # ? EH_String is not defined
                    ("after_expand", ct.POINTER(EH_String)), # ? EH_String is not defined
                    ("macrotype", c_int), # WHAT IS 'rplace'? 
                    ("macro_name_tokens", ct.POINTER(CToken)), # ? CToken is not defined
                    ("macro_afterexpand_tokens", ct.POINTER(CToken))] # ? CToken is not defined



