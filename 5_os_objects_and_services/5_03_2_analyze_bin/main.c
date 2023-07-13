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

//#define _LARGEFILE64_SOURCE
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
#define SYSCALL_OPEN_ERROR  (-1)
#define SYSCALL_READ_ERROR  (-1)
#define SYSCALL_READ_EOF    0
#define RD_BYTES_CNT        1
#define MAX_NUM_FOUR_BYTES  0xFFFFFFFFLU

enum binary_error {
    BINARY_ERROR_RD,
    BINARY_ERROR_NON_FOUR_BYTES,

    BINARY_ERROR_NUM,
};

struct bin_info {
    unsigned min;
    unsigned max;
    unsigned cnt;
    const char *filename;
    int fd;     /*file descriptor */
    enum binary_error error;
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

/**
 * @brief 
 * 
 */
static bool is_error(enum binary_error err);

/**
 * @brief Get the text error code string.
 * 
 * @param err enum binary_error.
 * @return char* Error code string.
 */
static const char * get_text_error_code(enum binary_error err);

static const char error_string_rd[] = "binary file read error";
static const char error_string_non_four_bytes[] = "binary file non four bytes";

static const char *error_strings[] = {
    [BINARY_ERROR_RD] = error_string_rd,
    [BINARY_ERROR_NON_FOUR_BYTES] = error_string_non_four_bytes,
};

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

    /* go through binary files */
    for (size_t i = 1; i < TEXT_FILE_INDX; i++) {
        struct bin_info bi;
        bi.filename = argv[i]; 
        bi.fd = open(bi.filename, O_RDONLY);
        bi.error = BINARY_ERROR_NUM;
        if (bi.fd == SYSCALL_OPEN_ERROR) {
            perror(bi.filename);
            // exit(1);
            continue;
        }
        bin_file_analyze(&bi);
        text_file_write_bin_info(file_text_wr, &bi);
        close(bi.fd);
    }

    fclose(file_text_wr);

    return 0;
}

static void bin_file_analyze(struct bin_info *bi)
{
    char arr[NUMBER_SIZE_BYTES];
    size_t i = 0;
    int rd;
    unsigned num;

    bi->max = 0;
    bi->min = MAX_NUM_FOUR_BYTES;
    bi->cnt = 0;

    while ((rd = read(bi->fd, (void *)&arr[i], RD_BYTES_CNT)) !=
            SYSCALL_READ_EOF) {
        if (rd == SYSCALL_READ_ERROR) {
            perror(bi->filename);
            bi->error = BINARY_ERROR_RD;
            return;
        }
        i++;
        if (i == NUMBER_SIZE_BYTES) {
            num = *(unsigned *)arr; /* big-endian */
            if (num > bi->max)
                bi->max = num;
            if (num < bi->min)
                bi->min = num;
            bi->cnt++;
            i = 0;
        }
    }
    if (i != 0)
        bi->error = BINARY_ERROR_NON_FOUR_BYTES;
}

static void text_file_write_bin_info(FILE *text_file, struct bin_info *bi)
{
    if (is_error(bi->error)) {
        const char * str_err = get_text_error_code(bi->error);
        fprintf(text_file, "%s: %s\n", bi->filename, str_err);
    } else {
        fprintf(text_file, "%s: cnt %u, min %u, max %u\n", 
                bi->filename, bi->cnt, bi->min, bi->max);
    }
}

static bool is_error(enum binary_error err)
{
    return (err == BINARY_ERROR_NUM) ? false : true;  
}

static const char * get_text_error_code(enum binary_error err)
{
    return error_strings[err];
}
