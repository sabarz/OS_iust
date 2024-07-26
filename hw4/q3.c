#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

int main() 
{    
    int pipes[5][2];
    int pid, hold = -1;

    for (int i = 0; i < 5; i++) 
    {
        pipe(pipes[i]);
        pid = fork();
    
        if (pid == 0)
        { 
            hold = i;
            break;
        }
    }

    if (pid == 0) 
    { 
        char str[2000];
        int n = getpid() % 5 + 2;

        for (int i = 0; i < n; i++) 
        {
            struct timeval t;
            gettimeofday(&t, NULL);
            if (i == n - 1) 
            {
                sprintf(str , "finished , time of finishing is : %ld" , t.tv_sec);
            }           
            write(pipes[hold][1], &str, sizeof(char) * 1024);   
            sleep(2); 
        }      
    } 
    else 
    { 
        bool finish[5];
        for(int i = 0 ; i < 5 ; i++)
        {
            finish[i] = false;
        }
        
        FILE* File = File = fopen("log.txt", "w+");
        fclose(File);

        FILE* f = fopen("log.txt", "a");
        int Count = 0;
        char str[1024];

        while (Count < 5) {
            for (int i = 0; i < 5; i++) 
            {
                if (!finish[i])
                {
                    read(pipes[i][0], &str, sizeof(char) * 1024);
                   
                    finish[i] = true;
                    Count++;
                
                    fputs(str, f);
                    fputs("\n", f);
                }
            }
        }
    }
    return 0;
}