#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>

struct shared_memory {
    int index;
    int print_index;
    char buf [10][256];
};

int main ()
{
    sem_t *mutex_sem, *buffer_count_sem, *spool_signal_sem;
    int fshm, flog;
    char buffer [256];
    struct shared_memory *memory_ptr;
    mutex_sem = sem_open ("/sem-mutex", O_CREAT, 0660, 0);
    fshm = shm_open ("/posix-shared-mem-example", O_RDWR | O_CREAT | O_EXCL, 0660);
    memory_ptr = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    flog = open ("/tmp/example.log", O_CREAT | O_WRONLY | O_APPEND | O_SYNC, 0666);

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
    if (memory_ptr == MAP_FAILED)
    {
        perror ("mmap");
        exit (1);
    }  
    if (flog == -1)
    {
        perror ("fopen");
        exit (1);
    }
    if (ftruncate (fshm, sizeof (struct shared_memory)) == -1)
    {
        perror ("ftruncate");
        exit (1);
    }    
  
    memory_ptr->index = memory_ptr->print_index = 0;

    if ((buffer_count_sem = sem_open ("/sem-buffer-count", O_CREAT | O_EXCL, 0660, 10)) == SEM_FAILED)
    {
        perror ("sem-open");
        exit (1);
    }
    if (sem_post (mutex_sem) == -1)
    {
        perror ("sem_post: mutex_sem");
        exit (1);
    }
    if ((spool_signal_sem = sem_open ("/sem-spool-signal", O_CREAT | O_EXCL, 0660, 0)) == SEM_FAILED)
    {
        perror ("sem-open");
        exit (1);
    }

    while (true) {  
        if (sem_wait (spool_signal_sem) == -1)
        {
            perror ("sem_wait: spool_signal_sem");
            exit (1);
        }    
        strcpy (buffer, memory_ptr -> buffer [memory_ptr -> print_index]);

        (memory_ptr -> print_index)++;
        if (memory_ptr -> print_index == 10)
           memory_ptr -> print_index = 0;

        if (sem_post (buffer_count_sem) == -1)
        {
            perror ("sem_post: buffer_count_sem");
            exit (1);
        }        
        if (write (flog, buffer, strlen (buffer)) != strlen (buffer))
        {
            perror ("write: logfile");
            exit (1);
        }  
  }
}
