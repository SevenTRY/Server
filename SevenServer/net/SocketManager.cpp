//
//  SocketManager.cpp
//  SevenClient
//
//  Created by  TianRunyan on 16/9/11.
//
//

#include "../lib/common.h"
#include <iostream>

void testNet(){
    int i,maxi,listendfd,connfd,sockfd;
    int nready;
    ssize_t n;
    char buf[MAXLINE];
    socklen_t chilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in chiladdr,seraddr;
    
    listendfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port  = htons(SERV_PORT);
    
    Bind(listendfd, (SA *)&seraddr, sizeof(seraddr));
    
    Listen(listendfd, LISTENQ);
    
    client[0].fd = listendfd;
    client[0].events = POLLRDNORM;
    
    for(i=1;i<OPEN_MAX;i++){
        client[i].fd = -1;
    }
    maxi = 0;
    
    for( ; ; ){

        nready = Poll(client, maxi+1, -1);
        if(client[0].revents & POLLRDNORM){
            chilen = sizeof(chiladdr);
            connfd = Accept(listendfd, (SA *)&chiladdr, &chilen);
            
            for(i=1;i<OPEN_MAX;i++){
                if(client[i].fd<0){
                    client[i].fd = connfd;
                    break;
                }
            }
            if(i == OPEN_MAX){
                err_quit("too many clients");
            }
            
            client[i] .events = POLLRDNORM;
            if(i>maxi){
                maxi = 1;
            }
            if(--nready <= 0){
                continue;
            }
        }
        
        for(i = 1;i<=maxi;i++){
            if((sockfd = client[i].fd) < 0){
                continue;
            }
            
            if(client[i].revents & (POLLRDNORM | POLLERR)){
                if((n = read(sockfd, buf, MAXLINE)) < 0){
                    if(errno == ECONNRESET){
                        Close(sockfd);
                        client[i].fd = -1;
                    }else{
                        err_sys("read error");
                    }
                }else if (n == 0){
                    Close(sockfd);
                    client[i].fd = -1;
                }else{
                    Writen(sockfd, buf, n);
                }
                
                if(--nready <= 0){
                    break;
                }
            }
        }
    }
}