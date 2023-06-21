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
#include <stdlib.h> /* for exit() */
#include <string.h>
#include <stdbool.h>

#define BLOCK_SIZE 1024

struct stream_poll {
    unsigned int cnt; /* counter of line characters */
    bool space;       /* space flag */
};

int main(int argc, char **argv)
{  
    if (argc < 4) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }

    struct stream_poll sp = {
        .cnt = 0,
        .space = false
    };
    int c;

    FILE *text_read = fopen(argv[1], "r");
    if (text_read == NULL) {
        perror(argv[1]);
        exit(1);
    }

    FILE *text_write = fopen(argv[2], "w"); /* truncate or create for writing */
    if (text_write == NULL) {
        perror(argv[2]);
        exit(2);
    }

    int binary_write = open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (binary_write == -1) {
	perror(argv[3]);
	exit(3);
    }

    while ((c = fgetc(text_read)) != EOF) {
        sp.cnt++;
        if (c == ' ' && sp.cnt == 1) /* new line */
            sp.space = true;

        if (c == '\n') { /* end of line */
            fputc(c, text_write);
            write(binary_write, &sp.cnt, sizeof(sp.cnt)); 
            sp.cnt = 0;
            sp.space = false;

            continue;
        } 

        if (sp.space)
            fputc(c, text_write);
    }

    fclose(text_read);
    fclose(text_write);
    close(binary_write);

    return 0;
}
