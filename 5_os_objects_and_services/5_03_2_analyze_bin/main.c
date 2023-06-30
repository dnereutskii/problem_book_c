/*
 * Task 5.03.2 (ref to 2.54)
 * 
 * The program reads binary files (filenames from arguments) and analyzes them. 
 * Creates text file (filename from last argument) into which content is
 * written.
 * Numbers in binary files are four bytes in size.
 * Text file content: filename | count of numbers | max | min
 * 
 * Note: For text files use high level input-output, for binary files
 *       use syscalls.
 *       Use bingen program in auxiliary.
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

#define MIN_CNT_ARGS        3
#define NUMBER_SIZE_BYTES   4
#define TEXT_FILE_INDX      (argc - 1)
#define SYSCALL_OPEN_ERROR  -1
#define SYSCALL_READ_ERROR  -1
#define SYSCALL_READ_EOF    0

struct bin_info {
    unsigned min;
    unsigned max;
    unsigned count;
    const char *filename;
    int fd;     /*file descriptor */
};

/**
 * @brief 
 * 
 * @param bi 
 */
static void bin_file_analyze(struct bin_info *bi);

/**
 * @brief 
 * 
 * @param text_file 
 * @param bi 
 */
static void text_file_write_bin_info(FILE *text_file, struct bin_info *bi);

int main(int argc, char **argv)
{  
    /* check count of args */
    if (argc < MIN_CNT_ARGS) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }

    /* create (truncate) text file */
    FILE *file_text_wr = fopen(argv[TEXT_FILE_INDX], "w");
    if (file_text_wr == NULL) {
        perror(argv[TEXT_FILE_INDX]);
        exit(1);
    }

    for (size_t i = 1; i < TEXT_FILE_INDX; i++) {
        struct bin_info bi;
        bi.filename = argv[i]; 
        bi.fd = open(bi.filename, O_RDONLY);
        if (bi.fd == SYSCALL_OPEN_ERROR) {
            perror(bi.filename);
            // exit(1);
            continue;
        }
        bin_file_analyze(&bi);
        text_file_write_bin_info(file_text_wr ,&bi);
        close(bi.fd);
    }


    return 0;
}

static void bin_file_analyze(struct bin_info *bi)
{
    char tmp[NUMBER_SIZE_BYTES];
    int rd;

    while ((rd = read(bi->fd, (void *)tmp, NUMBER_SIZE_BYTES)) != 
           SYSCALL_READ_EOF) {
        if (rd == SYSCALL_READ_ERROR)
            break;
        if (rd != NUMBER_SIZE_BYTES) {
            
        }
    }
}

static void text_file_write_bin_info(FILE *text_file, struct bin_info *bi)
{

}