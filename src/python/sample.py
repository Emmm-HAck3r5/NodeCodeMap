"""
THIS IS SAMPLE.PY
"""
import ctypes
print('SAMPLE.PY')

_path = 'C:\\Users\\hotbr\\Desktop\\Projects\\NodeCodeMap\\src\\public\\sample.so'
_mod = ctypes.cdll.LoadLibrary(_path)

gcd = _mod.gcd
gcd.argtypes = (ctypes.c_int, ctypes.c_int)
gcd.restype = ctypes.c_int
