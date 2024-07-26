#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>


int main() {
    sem_t *sem1 = sem_open("Hi", O_RDONLY, 0666, 0);

    int hold = 0; 
    for (int i = 1; i < 5; i++) 
    {
        int pid = fork();
        if (pid == 0) 
        {
            hold = i;
            break;
        }
    }

    sem_t *sem_i = sem_open(semNameGen(hold), O_RDONLY, 0666, 0);  
    int shm_fd = shm_open(shrNameGen(hold), O_RDWR, 0666);
    int* ptr = (int *)mmap(0, 10000, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    int l = 5;

    sem_wait(sem_i);

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < l - 1; j++) 
        {
            if (ptr[j] > ptr[j + 1]) 
            {
                int tmp = ptr[j];
                ptr[j] = ptr[j + 1];
                ptr[j + 1] = tmp;
            }
        }
    }
    sem_post(sem1);

    sem_close(sem1);
    sem_close(sem_i);

    munmap(ptr, 10000);
    close(shm_fd);
}