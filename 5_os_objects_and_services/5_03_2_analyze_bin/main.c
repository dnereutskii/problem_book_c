/*
 * Task 5.03.2 (ref to 2.54):
 *
 * ./prog -s 1 -n filename << stream of numbers
 * Note: For text files use high level input-output, for binary files
 *       use syscalls.
 * 
 */

#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h> /* for exit() */
#include <string.h>
#include <stdbool.h>

int main(int argc, char **argv)
{  
    if (argc < 2) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }

    FILE *file_text_wr = fopen(file_name, "w"); /* truncate or create for writing */
    if (file_text_wr == NULL) {
        perror(argv[2]);
        fclose(file_text_rd);
        exit(2);
    }
 
    int file_bin = open(file_name, O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (file_bin == -1) {
	    perror(argv[3]);
	    exit(3);
    }


    return 0;
}
