
import ctypes as ct
print('NCC PACKING...')

_path = 'C:\\Users\\hotbr\\Desktop\\Projects\\NodeCodeMap\\src\\c\\python_ncc.so'
ncc = ct.cdll.LoadLibrary(_path)

class NC_CFile(ct.Structure):
    pass
    
NC_CFile._fields_ = [("parent", ct.POINTER(NC_CFile)),
                    ("file_path", ct.c_char_p),
                    ("type", ct.c_int),
                    ("type_modifier", ct.c_ubyte),
                    ("lineno", ct.c_uint),
                    ("value", ct.POINTER(ct.c_int)),
                    ("data", ct.POINTER(ct.c_void_p))]
parse = ncc.Parse
parse.argtypes = ()
parse.restype = ct.POINTER(NC_CFile)

if __name__ == "__main__":
    file = parse()
    print("file->parent->type = " , file.contents.parent.contents.type)
    print("file->file_path = ", file.contents.file_path)
    print("file->type = ", file.contents.type)
    print("file->type_modifier = ", file.contents.type_modifier)
    print("file->value = ", file.contents.value.contents.value)
    print("file->data = ", file.contents.data.contents.value)
    print("file->lineno = ", file.contents.lineno)
