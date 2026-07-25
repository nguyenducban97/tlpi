/*************************************************************************\
*                  Copyright (C) Ban Nguyen 2026.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Exercises 4.1 */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE
    #define BUF_SIZE (1024)
#endif

static char buf[BUF_SIZE];

int main(int argc, char* argv[])
{
    int opt;
    int fd;
    int flags = O_WRONLY | O_CREAT | __O_CLOEXEC;
    mode_t mode = S_IRUSR | S_IWUSR;
    char *filespec = NULL;
    int numRead=0;
    int numWrite=0;

    while ((opt=getopt(argc, argv, ":a:help")) != -1)
    {
        switch(opt) 
        {
            case 'a':
                filespec=optarg;
                flags |= O_APPEND;
                break;
            case 'h':
                usageErr("using tee or tee -a file name");
                break;
            default:
                break;
        }
    }

    if(optind < argc)
    {
        filespec = argv[optind];
    }

    if(filespec != NULL)
    {
        fd = open(filespec, flags, mode);
        if (fd == -1)
            errExit("Open");
    }
    memset(buf,0,BUF_SIZE);
    while(-1 != (numRead =read(STDIN_FILENO, buf, BUF_SIZE - 1)))
    {
        if(numRead > 0)
        {
            buf[numRead]='\0';
            write(STDOUT_FILENO,buf,numRead);
            if(filespec != NULL)
            {
                numWrite = write(fd, buf,numRead);
                if (numRead != numWrite)
                {
                    errMsg("Cannot write full buffer!");
                }
            }
        }
        memset(buf,0,numRead);
    }

    return 0;
}

