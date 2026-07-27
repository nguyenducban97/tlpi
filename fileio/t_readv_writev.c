/*************************************************************************\
*                  Copyright (C) Ban Nguyen 2026.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Exercises 5.7*/

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>
#include "tlpi_hdr.h"

static int t_readv(int fd, const struct iovec *iov, int iovcnt);
static int t_writev(int fd, const struct iovec *iov, int iovcnt);

int main (int argc, char* argv[])
{
    int fd;
    struct iovec iov[3];

    return EXIT_SUCCESS;
}

static int t_readv(int fd, const struct iovec *iov, int iovcnt)
{
    int ret;
    int sum_len=0;
    int incre = 0;
    int diff = 0;
    int cp_len = 0;
    char *buf = NULL;

    for(int i=0; i < iovcnt; i++)
    {
        sum_len += iov[i]->iov_len;
    }
    buf = malloc(sum_len);
    if (buf == NULL) errExit("Malloc");
    ret = read(fd, buf, sum_len);

    if(ret > 0)
    {
        diff = ret;
        for(int i=0; i < iovcnt; i++)
        {
            cp_len = iov[i]->iov_len;
            if(diff < iov[i]->iov_len)
            {
                cp_len = diff;
            }

            memcpy(iov[i]->iov_base, buf[incre], cp_len);
            incre += cp_len;
            diff -= cp_len;
        }
    }
    else
    {
        errExit("Read error");
    }
    free(buf);

    return ret;
}

static int t_writev(int fd, const struct iovec *iov, int iovcnt)
{
    int ret;
    int sum_len=0;
    int incre = 0;
    int diff = 0;
    int cp_len = 0;
    char *buf = NULL;

    buf = malloc(sum_len);
    if (buf == NULL) errExit("Malloc");
    for(int i=0; i < iovcnt; i++)
    {
        memcpy(buf[sum_len], iov[i]->iov_base, iov[i]->iov_len);
        sum_len += iov[i]->iov_len;
    }

    ret = write(fd, buf, sum_len);
    free(buf);

    return ret;
}

