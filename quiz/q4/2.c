#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>

struct shared_memory {
    int index;
    int print_index;
    char buf [10] [256];
};

int main()
{
    struct shared_memory *shared_mem_ptr;
    int fshm;
    char mybuf [256];
    sem_t *mutex_sem, *buffer_count_sem, *spool_signal_sem;
    fshm = shm_open ("/posix-shared-mem-example", O_RDWR, 0);
    mutex_sem = sem_open ("/sem-mutex", 0, 0, 0);
    spool_signal_sem = sem_open ("/sem-buffer-count", 0, 0, 0);
    buffer_count_sem = sem_open ("/sem-buffer-count", 0, 0, 0);
    if ((shared_mem_ptr = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,fshm, 0)) == MAP_FAILED)
    {
        perror ("mmap");
        exit (1);
    }
    if (mutex_sem == SEM_FAILED)
    {
        perror ("sem-open");
        exit (1);
    }    
    if (fshm == -1)
    {
        perror ("shm-open");
        exit (1);
    }
    if (spool_signal_sem == SEM_FAILED)
    {      
        perror ("sem-open");
        exit (1);
    }
    if (buffer_count_sem == SEM_FAILED)
    {
        perror ("sem-open");
        exit (1);
    }

    char buf [300], *cp;

    printf ("type a message: ");

    while (fgets (buf, 198, stdin)) {
        int length = strlen (buf);
        if (buf [length - 1] == '\n')
           buf [length - 1] = '\0';

        if (sem_wait (buffer_count_sem) == -1)
        {
            perror ("sem_wait: buffer_count_sem");
            exit (1);
        }    
        if (sem_wait (mutex_sem) == -1)
        {
            perror ("sem_wait: mutex_sem");
            exit (1);
        }
            time_t now = time (NULL);
            cp = ctime (&now);
            int len = strlen (cp);
            if (*(cp + len -1) == '\n')
                *(cp + len -1) = '\0';
            sprintf (shared_mem_ptr -> buf [shared_mem_ptr -> buffer_index], "%d: %s %s\n", getpid(),cp, buf);
            (shared_mem_ptr -> buffer_index)++;
            if (shared_mem_ptr -> buffer_index == 10)
                shared_mem_ptr -> buffer_index = 0;

        if (sem_post (spool_signal_sem) == -1)
        {
            perror ("sem_post: (spool_signal_sem");
            exit (1);
        }
        if (sem_post (mutex_sem) == -1)
        {
            perror ("sem_post: mutex_sem");
            exit (1);
        }    
       
        printf ("type a message: ");
    }
 
    if (munmap (shared_mem_ptr, sizeof (struct shared_memory)) == -1)
    {
    perror ("munmap");
    exit (1);
    }
    exit (0);
}

