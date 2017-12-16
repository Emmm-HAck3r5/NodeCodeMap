#include <cstdlib>  
#include <cstdio>  
using namespace std;  
  
void value2str(int value)  
{  
    char str[30] = {'\0'};  
  
    itoa(value, str, 10);
    printf("string : %s\n", str);  
}  
  
void str2value(char* str)  
{  
    int value = atoi(str);  
    printf("value : %d\n", value);      
}  