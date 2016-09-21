//
//  sig_chld_waitpid.cpp
//  SevenServer
//
//  Created by tianrunyan on 16/9/21.
//  Copyright © 2016年 tianrunyan. All rights reserved.
//

#include "common.h"

void sig_chld(int signo)
{
    pid_t	pid;
    int		stat;
    
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated\n", pid);
    }
    return;
}
