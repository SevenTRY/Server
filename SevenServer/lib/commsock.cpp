//
//  commsock.cpp
//  SevenClient
//
//  Created by  TianRunyan on 16/9/11.
//
//

#include <stdio.h>
#include "common.h"

int Socket(int family,int type,int protocol){
    
    int n;
    if((n = socket(family, type, protocol))<0)
        err_sys("socket error");
    return n;    
}

void Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (connect(fd, sa, salen) < 0)
        err_sys("connect error");
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (bind(fd, sa, salen) < 0)
        err_sys("bind error");
    printf("bind succeed!\n");
}

/* include Listen */
void Listen(int fd, int backlog)
{
    char	*ptr;
    
    /*4can override 2nd argument with environment variable */
    if ( (ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);
    
    if (listen(fd, backlog) < 0)
        err_sys("listen error");
    
    printf("listening……\n");
}

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
           struct timeval *timeout)
{
    int		n;
    
    if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
        err_sys("select error");
    return(n);		/* can return 0 on timeout */
}

ssize_t Read(int fd, void *ptr, size_t nbytes)
{
    ssize_t		n;
    
    if ( (n = read(fd, ptr, nbytes)) == -1)
        err_sys("read error");
    return(n);
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int		n;
    
again:
    if ( (n = accept(fd, sa, salenptr)) < 0) {
#ifdef	EPROTO
        if (errno == EPROTO || errno == ECONNABORTED)
#else
            if (errno == ECONNABORTED)
#endif
                goto again;
            else
                err_sys("accept error");
    }
    return(n);
}

pid_t Fork(void)
{
    pid_t	pid;
    
    if ( (pid = fork()) == -1)
        err_sys("fork error");
    return(pid);
}

void Close(int fd)
{
    if (close(fd) == -1)
        err_sys("close error");
}
int
Poll(struct pollfd *fdarray, unsigned int nfds, int timeout)
{
    int		n;
    
    if ( (n = poll(fdarray, nfds, timeout)) < 0)
        err_sys("poll error");
    
    return(n);
}

