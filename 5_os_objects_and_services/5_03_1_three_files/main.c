/*
 * Task 5.03.1:
 *
 * Note: For text files use high level input-output, for binary files
 *       use syscalls.
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

#define BLOCK_SIZE 1024

int main(int argc, char **argv)
{  
    if (argc < 4) {
	fprintf(stderr, "too few arguments\n");
	return 1;
    }

    FILE *text = fopen(argv[1], "r");
    if (text == NULL) {
	fprintf(stderr, "no text file\n");
	return 0;
    } else {
	fclose(text);
    }

    return 0;
}
