//
//  signal.cpp
//  SevenServer
//
//  Created by tianrunyan on 16/9/21.
//  Copyright © 2016年 tianrunyan. All rights reserved.
//

#include "common.h"

Sigfunc * Signal(int signo, Sigfunc *func)	/* for our signal() function */
{
    Sigfunc	*sigfunc;
    
    if ( (sigfunc = signal(signo, func)) == SIG_ERR)
        err_sys("signal error");
    return(sigfunc);
}