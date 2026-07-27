/*************************************************************************\
*                  Copyright (C) Ban Nguyen 2026.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Exercises 5.3 */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "tlpi_hdr.h"

int main (int argc, char* argv[])
{
    int fd;
    Boolean useAppend = argc < 3;
    int flags = useAppend ? O_APPEND:0;
    int numBytes = getInt(argv[2], 0, "num-bytes");

    if(-1 == (fd =open(argv[1], O_WRONLY | O_CREAT | flags, S_IRUSR|S_IWUSR)))
    {
        errExit("Open");
    }

    for(int i = 0; i < numBytes; i++)
    {
        if(!useAppend)
        {
            if( -1 == lseek(fd, 0, SEEK_END))
                errExit("lseek");
        }
        if( 1 != write(fd, "x",1))
            fatal("write() failed");

    }

    printf("%ld done\n", (long) getpid());
    
    return 0;
}