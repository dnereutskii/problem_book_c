/*
 * Task 5.06
 * 
 * Encrypts data (from file) by XOR operation.
 * 
 * Call string: ./prog file_name key
 * key is 32-bit unsigned integer number.
 *
 * @author rhetti
 * @date 07.2023
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#define READ_BLOCK_SIZE     4096
#define WRITE_BLOCK_SIZE    4096
#define MIN_ARGS_CNT        3
#define BYTE_MAX_VALUE      255    
#define SYSCALL_OPEN_ERR    (-1)
#define SYSCALL_READ_EOF    (0)
#define SYSCALL_READ_ERR    (-1)
#define SYSCALL_WRITE_ERR   (-1)

enum {
    ARGS_INDX_FILE_NAME = 1,
    ARGS_INDX_KEY,
};
struct args_info {
    int fd;
    unsigned key; /* 32-bit unsigned integer */
};

int main(int argc, char **argv)
{
    if (argc < MIN_ARGS_CNT) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }

    struct args_info ainfo;
    
    /* open file (text or binary)*/
    char *file_name = argv[ARGS_INDX_FILE_NAME];
    ainfo.fd = open(file_name, O_WRONLY);
    if (ainfo.fd == SYSCALL_OPEN_ERR) {
        perror(file_name);
        return 1;
    }

    /* get key */
    if (sscanf(argv[ARGS_INDX_KEY], "%u", &ainfo.key) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_KEY]);
        return 1;
    }

    
    close(ainfo.fd);
    
    return 0;
}

