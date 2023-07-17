/*
 * Task 5.05
 *
 *
 * @author rhetti
 * @date 07.2023
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#define READ_BLOCK_SIZE     1024
#define MIN_ARGS_CNT        5
#define SYSCALL_OPEN_ERR    (-1)
#define SYSCALL_READ_EOF    (0)
#define SYSCALL_READ_ERR    (-1)

enum {
    ARGS_INDX_FILE_NAME = 1,
    ARGS_INDX_POS,
    ARGS_INDX_LEN,
    ARGS_INDX_BYTE,
};

int main(int argc, char **argv)
{
    if (argc < MIN_ARGS_CNT) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }

    char *file_name = argv[ARGS_INDX_FILE_NAME];
    int file = open(file_name, O_WRONLY);
    if (file == SYSCALL_OPEN_ERR) {
        perror(file_name);
        return 1;
    }

    unsigned pos;
    if (sscanf(argv[ARGS_INDX_POS], "%u", &pos) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_POS]);
        // perror(argv[ARGS_INDX_POS]);
        return 1;
    }

    unsigned len;
    if (sscanf(argv[ARGS_INDX_LEN], "%u", &len) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_LEN]);
        // perror(argv[ARGS_INDX_LEN]);
        return 1;
    }

    unsigned byte;
    if (sscanf(argv[ARGS_INDX_BYTE], "%u", &byte) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_BYTE]);
        // perror(argv[ARGS_INDX_BYTE]);
        return 1;
    }

    printf("%s %u %u %u", file_name, pos, len, byte);

    close(file);
    
    return 0;
}
