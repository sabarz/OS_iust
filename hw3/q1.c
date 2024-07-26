#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

typedef struct {int i ; int j; int index;} myarg_t;
double sum_of_threds[1000];

void* calculate_pi(void *arg)
{
    myarg_t *args = (myarg_t *) arg;
    int y = args->index; 
    for(int x = args->i ; x <= args->j ; x++)
    {
        //printf("%d" , x);
        if(x % 2 == 0)
            sum_of_threds[y] += ((double)1 / (double)((double)(2 * x) + 1));
        else
            sum_of_threds[y] -= ((double)1 / (double)((double)(2 * x) + 1));
    }
    printf("%f \n" , sum_of_threds[y]);
    pthread_exit(0) ; 
}
int main()
{
    int N , T;
    scanf("%d %d" , &N , &T);
   
    pthread_t t_id[T] ; 
    int size = (N + 1) / T ;
    int first = 0 , last = size;
    //printf("%d" , size);
    for(int i = 0 ; i < T ; i ++)
    {
        myarg_t arg = {first , last , i};
       
        pthread_create(&t_id[i] , NULL , &calculate_pi , &arg) ;
        pthread_join(t_id[i] , NULL);

        if(i == T - 2)
        {
            first = last + 1 ;
            last = N;
        }
        else
        {
            first = last + 1 ;
            last = first + size;  
        }
    }

    double ans = 0 ;
    for(int i = 0 ; i < T; i ++)
    {
        ans += sum_of_threds[i] ;
    }

    printf("%f \n" , ans) ;
}