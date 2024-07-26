#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>
#include <limits.h>
#include <stdbool.h>

int main() {
    sem_t *sem[5];
    int a[25];
    int shm_fd[5];
    int* ptr[5];
    int counters[5] = {0};
    int pIndex = 0;

    for (int i = 0; i < 5; i++) 
    {
        sem[i] = sem_open(semNameGen(i), O_CREAT, 0666, 0);
    }
    sem_t *sem2 = sem_open("Hi", O_CREAT, 0666, 0);

    srand(time(NULL));
    for (int i = 0; i < 25; i++)
    {
        a[i] = rand();
    }

    for(int i = 0 ; i < 25 ; i++)
    {
        printf("%d  " , a[i]);
    }

    for (int i = 0; i < 2; i++)
    {
        shm_fd[i] = shm_open(shrNameGen(i), O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd[i], 10000);
        ptr[i] = (int *)mmap(0, 10000, PROT_WRITE, MAP_SHARED, shm_fd[i], 0);
        
        for (int j = 0; j < 5; j++) 
        {
            ptr[i][j] = a[i * 5 + j];
        }
        sem_post(sem[i]);
    }
    for (int i = 2; i < 5; i++)
    {
        shm_fd[i] = shm_open(shrNameGen(i), O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd[i], 10000);
        ptr[i] = (int *)mmap(0, 10000, PROT_WRITE, MAP_SHARED, shm_fd[i], 0);
        
        for (int j = 0; j < 5; j++) 
        {
            ptr[i][j] = a[i * 5 + j];
        }
        sem_post(sem[i]);
    }
    
    for (int i = 0; i < 5; i++) 
    { 
        sem_wait(sem2); 
    } 

    while(true) 
    {
        int nmd = 0;
        for (int i = 0; i < 5; i++) 
        {
            if(a[i] == 5)
                nmd++;
        }
        if(nmd == 5)
        {
            break;
        }
        int min = 9999999;
        int index;
        for (int i = 0; i < 5; i++) 
        {
            if (counters[i] != 5 && ptr[i][counters[i]] < min)
            { 
                min = ptr[i][counters[i]];
                index = i;
            }
        }
        a[pIndex] = min;
        pIndex++;
        counters[index]++;
    }

    printf("sorted array : \n");
    for (int i = 0; i < 25; i++) 
    {
        printf("%d  ", a[i]);
    }

    for (int i = 0; i < 5; i++) 
    {
        munmap(ptr[i], 10000);
        close(shm_fd[i]);
        sem_close(sem[i]);
        sem_unlink(semNameGen(i));
    }
    sem_close(sem2);
    sem_unlink("Hi");
}