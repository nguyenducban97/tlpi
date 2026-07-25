/*************************************************************************\
*                  Copyright (C) Ban Nguyen 2026.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Exercises 4.2 */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "tlpi_hdr.h"

/*
Requirements:
    - if user don't enter any arguments, report "cp: missing file operand"
    - if user just enter source file, report "cp: missing destination file operand after 'filename'
    - if user enter more than 2 arguments, report "cp: target 'last_arguments' is not a directory"
    - if user enter a source file that is not existing, report cp: cannot stat 'filename': No such file or directory
    - other, copy source file to destination
*/

#ifndef BUF_SIZE
    #define BUF_SIZE (1024)
#endif

static char buf[BUF_SIZE];

int main (int argc, char* argv[])
{
    int src_fd, dest_fd;
    int nRead = 0;
    int nWrite = 0;

    if(argc == 1)
    {
        errExit("cp: missing file operand");
    }
    else if (argc == 2)
    {
        errExit("cp: missing destination file operand after %s", argv[1]);
    }
    else if (argc == 3)
    {
        src_fd = open(argv[1], O_RDONLY);
        if (src_fd == -1)
            errExit("Open source file");
        dest_fd = open(argv[2], O_WRONLY | O_TRUNC);
        if ((dest_fd == -1) && (errno == ENOENT))
        {
            dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
            if (-1 == dest_fd) errExit("Creat dest file");
        }
        else
        {
            errExit("Open dest file");
        }

        do
        {
            if(nRead > 0)
            {
                nWrite = write(dest_fd, buf, nRead);
                if( -1 == nWrite)
                {
                    errExit("Write to dest file");
                }
                else if (nWrite != nRead)
                {
                    errMsg("Just write %d byte instead of %d", nWrite, nRead);
                }
            }
            memset(buf, 0, BUF_SIZE);
            nRead = read(src_fd, buf, (BUF_SIZE -1));
        } while((-1 != nRead) && (0 != nRead));

        close(src_fd);
        close(dest_fd);
    }
    else
    {
        errExit("cp: target %s is not a directory", argv[argc-1]);
    }

    return 0;
}
