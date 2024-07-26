#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int first , last, *nums;

void Merge(int num[], int l, int m, int r)
{
   // printf("hi");
    int rr = r;
    int ll = l;
    int mm = m;
    int hold[r - l + 1];
    int p = 0;

    while (ll <= m - 1 && mm <= r)
    {
        if (num[ll] <= num[mm])
        {
            hold[p++] = num[ll++];
        }
        else
        {
            hold[p++] = num[mm++];
        }
    }

    while (ll <= m - 1)
    {
        hold[p++] = num[ll++];
    }
    while (mm <= r)
    {
        hold[p++] = num[mm++];
    }

    int j = 0;
    for (int i = l; i <= r; i++)
    {
        num[i] = hold[j++];
    }
}

void divide(int num[], int l, int r)
{
    while (l < r)
    {
        int middle = (l + r) / 2;
        divide(num, l, middle);
        divide(num, middle + 1, r);
        Merge(num, l, middle + 1, r);
        return;
    }
}
int main(int argc, char *argv[])
{
    int n , m ;
    char d;
    nums = malloc(n * sizeof(int));

    scanf("%d", &n); 
    scanf("%d" , &m);

    for(int i = 0 ; i < n ; i ++)
    {
        scanf("%d%c" , &nums[i],  &d);
    }

    pid_t pid = getpid();
    first = 0 , last = (n / m) - 1;
   
    for(int i = 0 ; i < m - 1; i ++)
    {
        vfork();
    }

    if(getpid() == pid)
    {
      //  printf("tof");
        divide(nums , 0 , n - 1);
    
        for(int i = 0 ; i < n ; i ++)
        {
            printf("%d ", nums[i]);
        }
        printf("\n");
    }
    else
    {
        divide(nums , first , last);
        first += (n / m) ;
        last += (n / m);
       // printf("toooof");
        exit(0);
    }

}