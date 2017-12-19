// 编译生成动态库: gcc -g -fPIC -shared -o libtest.so test.c  
  
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
typedef enum Day{
    SUM = 7,
    MON = 1,
    TUE = 2,
    WED = 3,
    THU = 4,
    FRI = 5,
    SAT = 6
}Day;
typedef struct StructPointerTest  
{  
    char name[20];  
    int age;
    Day day;
}StructPointerTest, *StructPointer;  
  
StructPointer test()    // 返回结构体指针  
{   
    StructPointer p = (StructPointer)malloc(sizeof(StructPointerTest));   
    strcpy(p->name, "Joe");  
    p->age = 20;
    p->day = SUM;
      
    return p;   
}