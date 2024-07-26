#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

typedef struct {int a ; int b;} mynewarg_t;
int POW, Val, matrix_size = 2, counter = 0;
int my_matrix[2][2];
pthread_mutex_t mutex;
pthread_cond_t cond_id[4] ;


void* calculate_matrix(void *arg)
{
    mynewarg_t* args = (mynewarg_t *) arg;

    pthread_mutex_lock(&mutex);
    //printf("hi");
    my_matrix[args->a][args->b] = Val;
    counter++;

    printf("new elemnt was added! \n");
    for(int i = 0 ; i < matrix_size ; i++)
    {
        for(int j = 0 ; j < matrix_size; j++)
        {
            printf("%d " , my_matrix[i][j]);
        }
        printf("\n");
    }       
    pthread_mutex_unlock(&mutex);

    printf("%d" , counter);
    if(counter == 4)
    {
        printf("hi");
        for(int i = 0 ; i < 4; i++)
        pthread_cond_signal(&cond_id[i]);
    }
    pthread_cond_wait(&cond_id[(args->a * 2) + args->b] , &mutex);

    pthread_mutex_lock(&mutex);
    printf("busy waiting finished! \n");

    int hold = my_matrix[args->a][args->b];
    //printf("%d\n" , hold);
    for(int i = 1 ; i <= POW - 1 ; i++)
    {
        hold *= (2 * hold);
    }
    //    printf("%d\n" , hold);
    my_matrix[args->a][args->b] = hold;
    printf("new elemnt was powered! \n");
    for(int i = 0 ; i < matrix_size ; i++)
    {
        for(int j = 0 ; j < matrix_size; j++)
        {
            printf("%d " , my_matrix[i][j]);
        }
        printf("\n");
    }  

    pthread_mutex_unlock(&mutex);
    //pthread_exit(NULL);

}
int main()
{
    scanf("%d %d" , &Val , &POW);

    pthread_t t_id[matrix_size * matrix_size];
    mynewarg_t args[4];
    pthread_mutex_init(&mutex , NULL);   

    int first = 0 , second = 0;
    for(int i = 0 ; i < 4; i++)
    {
        args[i].a = first;
        args[i].b = second;
        
        pthread_create(&t_id[i] , NULL , calculate_matrix , (void *)&args[i]);

        if(i + 1 == 2)
        {
            first++;
            second = 0;
        }
        else
        {
            second++;
        }
    }
    
    for (int i = 0; i < matrix_size * matrix_size; i++)
        pthread_cond_init(&cond_id[i] , NULL);

    for (int i = 0; i < matrix_size * matrix_size; i++)
        pthread_join(t_id[i], NULL);

    pthread_mutex_destroy(&mutex);


}