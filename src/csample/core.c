#include "core.h"
int sum(int n,int h,int *arr)
{
    int ans=0;
    int i=0;
    for (i=0;i<n;i++)
    {
        if (arr[i]>=h)
            ans++;
    }
    return ans;
}