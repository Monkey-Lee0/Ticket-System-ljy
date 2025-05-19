#ifndef TOOLS_H
#define TOOLS_H

#include<string>
#include"database.h"

template<int len>
char* my_c_str(const std::string &a)
{
    static char s[len];
    for(int i=0;i<len;i++)
    {
        if(a.size()>i)
            s[i]=a[i];
        else
            s[i]='\0';
    }
    return s;
}

inline std::string decompose(std::string &str)
{
    std::string tmp;
    while(!str.empty()&&str.back()!=' '&&str.back()!='\n'&&str.back()!='\r')
        tmp.push_back(str.back()),str.pop_back();
    while(!str.empty()&&(str.back()==' '||str.back()=='\n'||str.back()=='\r'))
        str.pop_back();
    return tmp;
}

#endif
