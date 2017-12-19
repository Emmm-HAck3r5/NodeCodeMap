#!/bin/env python  
# coding=UTF-8  
  
from ctypes import *  
  
#python中结构体定义  
class StructPointer(Structure):  
    _fields_ = [("name", c_char * 20),
                ("age", c_int),
                ("day", c_int)]  
  
if __name__ == "__main__":  
    lib = cdll.LoadLibrary('C:\\Users\\hotbr\\Desktop\\Projects\\NodeCodeMap\\src\\public\\libtest.so')  
    lib.test.restype = POINTER(StructPointer)  
    p = lib.test()  
  
    print(p.contents.name, p.contents.age, p.contents.day)