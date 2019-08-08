#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>

#define MAXLINE 4096
#define DEFAULT_PORT 8894

/**
 * socket实现进程通信
 * 
 */
int main(int argc, char *argv[]){
    int socket_fd;
    int connect_fd;
    struct sockaddr_in servaddr;
    char buf[MAXLINE];
    int n;

    //init socket
    if((socket_fd=socket(AF_INET, SOCK_STREAM, 0)) == -1){
        printf("create socket error: %s(errno: %d)", strerror(errno),errno);
        exit(0);
    }

    memset(&servaddr,0,sizeof(servaddr));
    
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //系统获取本机ip
    servaddr.sin_port = htons(DEFAULT_PORT);
    servaddr.sin_family = AF_INET;

    if(bind(socket_fd, &servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n", strerror(errno),errno);
        exit(0);
    }
    
    if(listen(socket_fd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    printf("======waiting for client's request======\n");

    while (1){
        if((connect_fd=accept(socket_fd, NULL, NULL)) == -1) {
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);  
            continue;  
        }

        if(!fork()){
            char msg[] = "Welcome to LFY shell !";
            if(send(connect_fd, msg, sizeof(msg), 0) < 0){
                printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
            }
            while(1){
                char res[MAXLINE];
                n = recv(connect_fd, buf, MAXLINE, 0);
                buf[n] = '\0';

                FILE *fp=popen(buf,"r");
                while(fgets(res,sizeof(res),fp) != NULL){
                    if (res[strlen(res) - 1] == '\n') {
                        res[strlen(res) - 1] = '\0';
                    }

                    if(send(connect_fd, res, sizeof(res), 0) < 0){
                       printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
                    }
                    printf("%s",res);
                }
                
            }
            close(connect_fd);  
        }
    }
    close(socket_fd);
}