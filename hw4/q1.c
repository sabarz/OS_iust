#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <stdbool.h>

#define size 9999
char *path = "/SHM";
struct prime
{
    int value;
    int counter;
};

bool is_prime(int x)
{
    for (int i = 2; i * i <= x; i++)
    {
        if (x % i == 0)
            return false;
    }

    return true;
}
int main(int argc, char *argv[])
{
    sem_t *mutex;
    char *w;
    int fd;
    mutex = (sem_t *) mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    sem_init(mutex, 1, 0);
    
    for(int i = 0 ; i < 8 ;i++)
    {
        int hold = fork();
        if(hold > 0)
        {
            fd = shm_open(NULL,O_RDWR | O_CREAT, 0666);
            if(fd >= 0)
            {
                ftruncate(fd, size);
                w = (struct prime *) mmap(0, size, PROT_WRITE | PROT_READ , MAP_SHARED, fd, 0);
                int c = 0 , x = 0;
                struct prime *q ;

                if(w != MAP_FAILED)
                {
                    while(c <= 1000)
                    {
                        bool check = is_prime(x);
                        if(check)
                        {
                            q->value = x;
                            q->counter = ++c;
                        }
                        else
                        {
                            printf("not a prime");
                        }
                        x ++;          
                    }  
                    w += i * 125;
                    sem_post(mutex);
                    wait(NULL); 
                }
            }
        }
        else if(hold == 0)
        {
            sem_wait(mutex); 
            fd = shm_open(path, O_RDONLY, 06666);
            if(fd >= 0)
            {
                w = (struct prime *) mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
                w += i * 125;
                int nmd = ((struct prime *)w)->counter;
                int g = i;
                
                while(true)
                {
                    if(nmd <= 0)
                    {
                        break;
                    }
                    else
                    {
                        ((struct prime *)w)->value + i;
                        ((struct prime *)w)->counter--;
                        w += (g+1) * 125;
                        g++ ;
                        nmd = ((struct prime *)w)->counter;
                    }
                }
            }
        }
    } 

    for(int i=0;i<8;i++)
        wait(NULL);

    return 0;

}