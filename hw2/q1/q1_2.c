#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/wait.h>
#include<sys/types.h>

int first , last, *nums;

typedef struct {int a ; int b;} myarg_t;

void Merge(int nums[], int l, int m, int r)
{
   // printf("hi");
    int rr = r;
    int ll = l;
    int mm = m;
    int hold[r - l + 1];
    int p = 0;

    while (ll <= m - 1 && mm <= r)
    {
        if (nums[ll] <= nums[mm])
        {
            hold[p++] = nums[ll++];
        }
        else
        {
            hold[p++] = nums[mm++];
        }
    }

    while (ll <= m - 1)
    {
        hold[p++] = nums[ll++];
    }
    while (mm <= r)
    {
        hold[p++] = nums[mm++];
    }

    int j = 0;
    for (int i = l; i <= r; i++)
    {
        nums[i] = hold[j++];
    }
}

void *divide(void *arg)
{
    //printf("hi");
    myarg_t *args = (myarg_t *) arg;
    int l = args->a;
    int r = args->b;
    //printf("%d  %d \n" , l , r);
    //printf("hsxhsu");
    while (l < r)
    {
        int middle = (l + r) / 2;
        myarg_t ar1 = {l , middle};
        // printf("yiiiii");
        divide(&ar1);
        myarg_t ar2 = {middle + 1 , r};
        divide(&ar2);
        Merge(nums, l, middle + 1, r);
        break;
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

    first = 0;
    last = (n / m) - 1;
    myarg_t arg = {first , last};

    pthread_t pid[m] ;
    //printf("hi");
    
    for(int i = 0 ; i < m - 1; i ++)
    {
        pthread_create(&pid[i] , NULL , *divide, &arg) ;
        pthread_join(pid[i] , NULL);
        first += (n / m) ;
        last += (n / m);
        myarg_t arg = {first , last};
        //printf("hi");
    }
    pthread_t pidL ;
    myarg_t ar3 = {0 , n - 1};
    divide(&ar3);

    for(int i = 0 ; i < n ; i ++)
    {
        printf("%d ", nums[i]);
    }
    printf("\n");

}