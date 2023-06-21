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

struct stream_poll {
    unsigned int cnt; /* counter of line characters */
    bool space;       /* space flag */
};

static const char file_text_wr_std_name[] = "text_mod.txt"; 
static const char file_bin_std_name[] = "bin_cnt"; 

int main(int argc, char **argv)
{  
    if (argc < 2) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }

    struct stream_poll sp = {
        .cnt = 0,
        .space = false
    };
    int c;
    const char *file_name;

    /* first file open */
    FILE *file_text_rd = fopen(argv[1], "r");
    if (file_text_rd == NULL) {
        perror(argv[1]);
        exit(1);
    }

    /* second file open */
    if (argc < 3)
        file_name = file_text_wr_std_name;
    else
        file_name = argv[2];
    FILE *file_text_wr = fopen(file_name, "w"); /* truncate or create for writing */
    if (file_text_wr == NULL) {
        perror(argv[2]);
        fclose(file_text_rd);
        exit(2);
    }
 
    /* third file open */
    if (argc < 4)
        file_name = file_bin_std_name;
    else
        file_name = argv[3];
    int file_bin = open(file_name, O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (file_bin == -1) {
	    perror(argv[3]);
        fclose(file_text_rd);
        fclose(file_text_wr);
	    exit(3);
    }

    /* chars handler */
    while ((c = fgetc(file_text_rd)) != EOF) {
        sp.cnt++;
        if (c == ' ' && sp.cnt == 1) /* new line */
            sp.space = true;

        if (c == '\n') {  /* end of line */
            if (sp.space) /* writeble line */
                fputc(c, file_text_wr);
            write(file_bin, &sp.cnt, sizeof(sp.cnt)); 
            sp.cnt = 0;
            sp.space = false;
            continue;
        } 

        if (sp.space)
            fputc(c, file_text_wr);
    }

    fclose(file_text_rd);
    fclose(file_text_wr);
    close(file_bin);

    return 0;
}
