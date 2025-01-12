#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<pthread.h>

// in your browser type: http://localhost:8090
// IF error: address in use then change the PORT number
#define PORT 8090

typedef struct argo{int new_socket ; char *hello ;} argum;

void *thread_func(void *arg)
{
    argum *my_arg = (argum *)arg;
    char buffer[30000] = {0};
    long valread = read(my_arg->new_socket , buffer, 30000);
    printf("%s\n",buffer );
    // uncomment following line and connect many clients
    sleep(5);
    write(my_arg->new_socket , my_arg->hello , strlen(my_arg->hello));
    printf("-------------Hello message sent---------------");
    close(my_arg->new_socket);
}
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        argum args = {new_socket , hello} ;
        pthread_t tid ;
        pthread_create(&tid , NULL , *thread_func , &args) ;
        pthread_join(tid , NULL);
    }
    return 0;
}

