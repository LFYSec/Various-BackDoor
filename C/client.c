#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>

#define MAXLINE 4096

int main(int argc, char *argv[]){
    int socket_fd, n, recv_len;
    char recvline[MAXLINE], sendline[MAXLINE];
    struct sockaddr_in servaddr;
    
    if(argc != 3){
        printf("usage");
        exit(0);
    }

    if((socket_fd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        printf("socket errorL: %s(error: %d)", strerror(errno),errno);
        exit(0);
    }

    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));

    if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr.s_addr) < 0){
        printf("inet_pton error! %s", strerror(errno));
        exit(0);
    }

    if(connect(socket_fd, &servaddr, sizeof(servaddr)) < 0){
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
        exit(0);  
    }

    printf("send msg to server: \n");
    while (1){
        printf("Receive: \n");
        while ((recv_len = recv(socket_fd, recvline, MAXLINE, 0)) != -1){
            recvline[recv_len] = '\0';
            printf("%s\n", recvline);  
        }
        

        printf("Send: ");
        fgets(sendline,MAXLINE,stdin);
        
        if(send(socket_fd, sendline, strlen(sendline), 0) < 0){
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
            exit(0);
        }
    }
    close(socket_fd);
    exit(0);
}