#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(int argc , char **argv)
{
    int hold = fork();

    if(hold == 0)
    {
        char *array[3];
        array[0] = "cat" ;
        array[1] = argv[1];
        array[2] = NULL;
        execvp(array[0] , array) ;
    }
    else
    {
        wait(0);
    }
}