#pragma GCC optimize("Ofast")
#include<cstdio>
#include"database.h"
B_Plus_Tree<char[65],int> BPT("Data");

int main()
{
    int n;
    scanf("%d",&n);
    while(n--)
    {
        char op[20],s[65];
        int val;
        scanf("%s%s",op,s);
        if(op[0]=='i')
        {
            scanf("%d",&val);
            BPT.Insert(s,val);
        }
        else if(op[0]=='d')
        {
            scanf("%d",&val);
            BPT.Delete(s,val);
        }
        else
        {
            if(auto res=BPT.Find(s); res.empty())
                puts("null");
            else
            {
                for(const auto t:res)
                    printf("%d ",t);
                puts("");
            }
        }
    }

    return 0;
}