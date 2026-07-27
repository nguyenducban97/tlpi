/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2026.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 5-2 */

#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifdef MYOWN_IPLM
static ssize_t t_readv(int fd, const struct iovec *iov, int iovcnt);
static ssize_t t_writev(int fd, const struct iovec *iov, int iovcnt);
#endif

int
main(int argc, char *argv[])
{
    int fd;
    struct iovec iov[3];
    struct stat myStruct;       /* First buffer */
    int x;                      /* Second buffer */
#define STR_SIZE 100
    char str[STR_SIZE];         /* Third buffer */
    ssize_t numRead, totRequired, numWrite;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    fd = open(argv[1], O_RDWR);
    if (fd == -1)
        errExit("open");

    totRequired = 0;

    iov[0].iov_base = &myStruct;
    iov[0].iov_len = sizeof(struct stat);
    totRequired += iov[0].iov_len;

    iov[1].iov_base = &x;
    iov[1].iov_len = sizeof(x);
    totRequired += iov[1].iov_len;

    iov[2].iov_base = str;
    iov[2].iov_len = STR_SIZE;
    totRequired += iov[2].iov_len;

#ifdef MYOWN_IPLM
    numRead = t_readv(fd, iov, 3);
#else
    numRead = readv(fd, iov, 3);
#endif
    if (numRead == -1)
        errExit("readv");

    if (numRead < totRequired)
        printf("Read fewer bytes than requested\n");

    printf("total bytes requested: %ld; bytes read: %ld\n",
            (long) totRequired, (long) numRead);

#ifdef MYOWN_IPLM
    lseek(fd, 0, SEEK_END);
    numWrite = t_writev(fd, iov, 3);
    if (numWrite == -1)
        errExit("writev");

    if (numWrite < totRequired)
        printf("Write fewer bytes than requested\n");

    printf("total bytes requested: %ld; bytes write: %ld\n",
            (long) totRequired, (long) numWrite);
#endif /*MYOWN_IPLM*/

    close(fd);
    exit(EXIT_SUCCESS);
}

#ifdef MYOWN_IPLM
static ssize_t t_readv(int fd, const struct iovec *iov, int iovcnt)
{
    int ret;
    ssize_t sum_len=0;
    ssize_t incre = 0;
    ssize_t diff = 0;
    ssize_t cp_len = 0;
    char *buf = NULL;

    for(int i=0; i < iovcnt; i++)
    {
        sum_len += iov[i].iov_len;
    }
    buf = malloc(sum_len);
    if (buf == NULL) errExit("Malloc");
    ret = read(fd, buf, sum_len);

    if(ret > 0)
    {
        diff = ret;
        for(int i=0; i < iovcnt; i++)
        {
            cp_len = iov[i].iov_len;
            if(diff < iov[i].iov_len)
            {
                cp_len = diff;
            }

            memcpy(iov[i].iov_base, &buf[incre], cp_len);
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

static ssize_t t_writev(int fd, const struct iovec *iov, int iovcnt)
{
    int ret;
    ssize_t sum_len=0;
    char *buf = NULL;

    buf = malloc(sum_len);
    if (buf == NULL) errExit("Malloc");
    for(ssize_t i=0; i < iovcnt; i++)
    {
        memcpy(&buf[sum_len], iov[i].iov_base, iov[i].iov_len);
        sum_len += iov[i].iov_len;
    }

    ret = write(fd, buf, sum_len);
    free(buf);

    return ret;
}

#endif /*MYOWN_IPLM*/