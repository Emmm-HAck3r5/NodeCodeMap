#include <string>  
#include <sstream>  
  
using namespace std;  
  
string ValueToStr(int value)  
{  
    ostringstream ost;  
    ost << value;  
  
    return ost.str();  
}  
  
int StrToValue(char* str)  
{  
    string strValue(str); 
    istringstream osValue;  
    osValue.str(strValue);
    
    int value;  
    osValue >> value; 
  
    return value;  
}  