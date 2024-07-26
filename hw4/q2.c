#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

//https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwjeh4aewab8AhWxxwIHHUt3AVIQFnoECAoQAQ&url=https%3A%2F%2Flinux.die.net%2Fman%2F2%2Fprocess_vm_writev&usg=AOvVaw2z0o1DAKpjHn7LXbqEVQvd
//GigaByte = 2 ^ 3 * 2 ^ 27 = 2 ^ 30
#define GigaByte pow(2 , 30)

int main(void)
{
    char characters[] = {'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' , 'I' , 'J' , 'K' , 
                'L' , 'M' , 'N' , 'O' , 'P', 'Q' , 'R' , 'S' , 'T' , 'U' , 'V' , 'W' , 'Y' , 'Z'};
  
    struct iovec iov[2];
    struct iovec remote[2];
    int fd[2];
    pipe(fd);
    size_t number_of_data = (float)3 / sizeof(char) * GigaByte; 
    int *my_Data = (int *)malloc(sizeof(char) * number_of_data);
    int *written_Data;

    pid_t pid = fork();

    if (pid == 0)
    {
        int *data = (int *)malloc(sizeof(char) * (size_t)number_of_data);
        write(fd[1], &data, sizeof(char *));

        sleep(5);
        exit(0);
    }
    
    read(fd[0], &written_Data, sizeof(char *));
    memset(my_Data, characters[rand() % 25], sizeof(char) * (size_t)number_of_data);

    iov[0].iov_base = &my_Data[0];
    iov[0].iov_len = 2 * number_of_data;
    remote[0].iov_base = &written_Data[0];
    remote[0].iov_len = 2 * number_of_data;

    iov[1].iov_base = &my_Data[2 * number_of_data];
    iov[1].iov_len = 2 * number_of_data;
    remote[1].iov_base = &written_Data[2 * number_of_data];
    remote[1].iov_len = 2 * number_of_data;

    int wcount;
    wcount += process_vm_writev(pid, &iov[0], 1, &remote[0], 1, 0);
    wcount += process_vm_writev(pid, &iov[1], 1, &remote[1], 1, 0);

    if (wcount == number_of_data)
    {
        printf("we have 3Gb data!");
    }

    sleep(5);
    wait(NULL);
}