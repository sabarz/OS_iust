#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(int argc , char **argv)
{
    int hold = fork();

    if(hold == 0)
    {
        char hold[1000] = "cat ";
        int index = 4;
        while(*argv[1] != '\0')
        {
            hold[index] = *argv[1];
            index++;
            argv[1]++; 
        }
        system(hold);
    }
    else
    {
        wait(0);
    }
}