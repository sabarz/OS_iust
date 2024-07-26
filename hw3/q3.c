#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<stdbool.h>

// 0 to thinking 
// 1 to hungry
// 2 to eating
//  https://www.ecb.torontomu.ca/~courses/coe518/Labs/lab4/lisi.edu-dining-Philosopherecture8.pdf

int phi = 5;
pthread_mutex_t mutex ;
pthread_cond_t cond_id[1000] ;
int condition[1000];

void think(int i)
{
    condition[i] = 0;  
    printf("philosoper %d is thinking. \n" , i);  
}

bool can_eat(int i)
{
    if(condition[i] == 1 && condition[(i + 1) % phi] != 2 && condition[(i + 4) % phi] != 2)
    {
        condition[i] = 2;
        pthread_cond_signal(&cond_id[i]);
        return true;
    }
    return false;
}

void pick_up_chap(int i)
{
    pthread_mutex_lock(&mutex);

    condition[i] = 1;  
    if(!can_eat(i))
    {
        pthread_cond_wait(&cond_id[i] , &mutex);
    }
    printf("philosoper %d is eating. \n" , i);

    pthread_mutex_unlock(&mutex);
}

void put_down_chap(int i)
{
    pthread_mutex_lock(&mutex);
   
    think(i);
    can_eat((i + 1) % phi);
    can_eat((i + 4) % phi);

    pthread_mutex_unlock(&mutex);
}

void* solve(void *c)
{
    int *cnt = (int *) c;
    for(int i = 0 ; i < 100 ; i++)
    {
        sleep(1.5);
        pick_up_chap(*cnt);
        sleep(0.5);
        put_down_chap(*cnt);
    }
}

int main()
{
    pthread_t t_id[phi]; 
    int w[phi];
    pthread_mutex_init(&mutex , NULL);   

    for(int i = 0 ; i < phi ; i++)
    {
        think(i);
        w[i] = i;
        pthread_cond_init(&cond_id[i] , NULL);
    } 
    
    for(int i = 0 ; i < phi ; i ++)
    {
        pthread_create(&t_id[i] , NULL , &solve , &w[i]);
    }
    for(int i = 0 ; i < phi ; i ++)
    {
        pthread_join(t_id[i] , NULL);
    }
    

}