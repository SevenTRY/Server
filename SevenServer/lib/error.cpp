//
//  error.cpp
//  SevenClient
//
//  Created by  TianRunyan on 16/9/11.
//
//

#include <stdio.h>
#include "common.h"
#include <stdarg.h>
#include <syslog.h>

int		daemon_proc;		/* set nonzero by daemon_init() */

static void	err_doit(int,int, const char *, va_list);

/* Fatal error related to system call
 * Print message and terminate */
void err_sys(const char *fmt, ...)
{
    va_list		ap;
    
    va_start(ap, fmt);
    err_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}


/* Fatal error unrelated to system call
 * Print message and terminate */

void err_quit(const char *fmt, ...)
{
    va_list		ap;
    
    va_start(ap, fmt);
    err_doit(0, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}


static void err_doit(int errnoflag, int level, const char *fmt, va_list ap){
    int		errno_save, n;
    char	buf[MAXLINE + 1];
    
    errno_save = errno;		/* value caller might want printed */
#ifdef	HAVE_VSNPRINTF
    vsnprintf(buf, MAXLINE, fmt, ap);	/* safe */
#else
    vsprintf(buf, fmt, ap);					/* not safe */
#endif
    n = strlen(buf);
    if (errnoflag)
        snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
    strcat(buf, "\n");
    
    if (daemon_proc) {
        syslog(level, buf);
    } else {
        fflush(stdout);		/* in case stdout and stderr are the same */
        fputs(buf, stderr);
        fflush(stderr);
    }
    return;
}