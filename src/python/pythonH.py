import ctypes as ct

print("PYTHONh PACKING...")
_path = 'C:\\Users\\hotbr\\Desktop\\Projects\\NodeCodeMap\\src\\c\\pythonH.dll'
pythonH = ct.cdll.LoadLibrary(_path)


class NC_CFile(ct.Structure):
    pass
    
NC_CFile._fields_ = [("parent", ct.POINTER(NC_CFile)),
                    ("file_path", ct.c_char_p),
                    ("type", ct.c_int),
                    ("type_modifier", ct.c_ubyte),
                    ("lineno", ct.c_uint),
                    ("value", ct.POINTER(ct.c_uint8)),
                    ("data", ct.POINTER(ct.c_void_p))]
parse = pythonH.Prase
parse.argtypes = ()
parse.restype = ct.POINTER(NC_CFile)


if __name__ == "__main__":
    root = parse()
    print(root.contents.parent.contents.file_path)
    print(root.contents.file_path)
    print(root.contents.parent.contents.type)
    print(root.contents.type)

