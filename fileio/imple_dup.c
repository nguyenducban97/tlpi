/*************************************************************************\
*                  Copyright (C) Ban Nguyen 2026.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Exercises 5.4 - 5.5*/

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "tlpi_hdr.h"

#define NEW_FD (58)

static int _dup_ (int oldfd);
static int _dup2_ (int oldfd, int newfd);
static void test_dup(void);
static void test_dup2(void);

int main (int argc, char* argv[])
{
    test_dup();
    test_dup2();

    return 0;
}

static int _dup_ (int oldfd)
{
    return (fcntl(oldfd, F_DUPFD, 0));
}

void test_dup (void)
{
    int fd, fd2;

    fd = open("./test_dup.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
    if(-1 == fd) errExit("Open fd");
    if( -1 == write(fd,"This is initial test message", 21))
        errExit("Error on write");
    fd2 = _dup_(fd);
    if( -1 == fd2) errExit("duplicate fd");
    /*Using fd2 change file offet to begining, then perform write using fd2*/
    lseek(fd2, 4, SEEK_SET);
    if( -1 == write(fd2, "THIS IS TEST MESSAGING USING fd2", 33))
        err_exit("Error on write using fd2");
    /*Using fd change file offet to begining, then perform write using fd*/
    lseek(fd, 0, SEEK_END);
    if( -1 == write(fd, "this is test message using fd", 30))
        err_exit("Error on write using fd");
    close(fd);
    close(fd2);
}

static int _dup2_ (int oldfd, int newfd)
{
    if(oldfd != newfd)
    {
        if(-1 != fcntl(newfd, F_GETFL))
            close(newfd);
        return(fcntl(oldfd, F_DUPFD, newfd));
    }
    else
    {
        return oldfd;
    }
}

static void test_dup2(void)
{
    int fd, fd2;
    char* buf[50];

    fd = open("./test_dup.txt", O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR);
    if(-1 == fd) errExit("Open fd");
    fd2 = _dup2_(fd, (int) NEW_FD);
    if( -1 == fd2)
        errExit("_dup2_ error");
    else
        errMsg("Expect %d, actualy %d", NEW_FD, fd2);
    /*Check the new descriptor has same flags as the old one by invoking read*/
    if (-1 == read(fd2, buf, 50))
        errMsg("Try read on the O_WRONLY desciptor");
    /*Check new descriptor share the same file offset*/
    lseek(fd2, 4, SEEK_SET);
    if(-1 == write(fd, "__dup2__", 9))
        errExit("Write using fd");
    close(fd);
    close(fd2);
}

