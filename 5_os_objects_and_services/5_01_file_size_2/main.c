/*
 * Task 5.01
 * 
 *
 */

#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char **argv)
{
    struct stat finfo;
    
    if (argc < 2)
	return 1;
    stat(argv[1], &finfo);

    printf("%ld\n", finfo.st_size);

    return 0;
}
