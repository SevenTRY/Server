//
//  SocketManager.cpp
//  SevenClient
//
//  Created by  TianRunyan on 16/9/11.
//
//

#include "../lib/common.h"

void testNet(){
    
    int listenfd,connfd;
    pid_t childpid;
    socklen_t chilen;
    
    struct sockaddr_in chiladdr,servaddr;
    
    listenfd  =  Socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    
    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    
    Listen(listenfd, LISTENQ);
    
    while(true){
        chilen = sizeof(chiladdr);
        connfd = Accept(listenfd, (SA *)&chiladdr, &chilen);
        printf("accpet");
        if((childpid = Fork()) == 0){
            printf("fork()");
            Close(childpid);
            str_echo(connfd);
            exit(0);
        }
        Close(connfd);
    }
    
}