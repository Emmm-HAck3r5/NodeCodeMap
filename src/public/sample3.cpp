template <typename T>  
string Value2Str(T value)  
{  
    ostringstream ost;  
    ost << value;  
    return ost.str();  
}  
  
template<typename T>   
T Str2Value(char* str)  
{  
    string strValue = str; 
    istringstream osValue;  
    osValue.str(strValue); 
  
    T value;  
    osValue >> value; 
  
    return value;  
} 
namespace boost  
{  
    class bad_lexical_cast;  
    template<typename Target, typename Source>  
    Target lexical_cast(const Source& arg);  
} 