/*************************************************************************\
*                  Copyright (C) Ban Nguyen 2026.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Exercises 5.2 */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "tlpi_hdr.h"

int main (int argc, char* argv[])
{
    int fd;
    char buf[]="Test data write after seek with O_APPEND flags";

    if(-1 == (fd = open("./test_seek_append.txt", O_WRONLY | O_APPEND)))
    {
        errExit("Recheck the test_seek_append.txt, if it is not exist, create it with some data for testing");
    }

    lseek(fd, 0, SEEK_SET);

    write(fd, buf, 50);
    close(fd);

    return 0;
}
