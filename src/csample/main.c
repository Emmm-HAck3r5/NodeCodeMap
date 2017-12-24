#include "core.h"
int main()
{
    int n=0,h=0,ans=0;
    int *arr=(int*)malloc(sizeof(int)*1000);
    scanf("%d %d",&n,&h);
    int i=0;
    for (i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }
    printf("%d",sum(n,h,arr));
    return 0;
}

