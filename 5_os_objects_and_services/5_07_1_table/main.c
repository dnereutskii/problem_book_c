/*
 * Task 5.07
 * 
 * 
 * 
 * Call string: ./table file_name cmd id
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

/* SYSCALLS defines */
#define SYSCALL_OPEN_ERR    (-1)
#define SYSCALL_READ_EOF    (0)
#define SYSCALL_READ_ERR    (-1)
#define SYSCALL_WRITE_ERR   (-1)

/* USER defines */
#define RDWR_BLOCK_SIZE     4096
#define MIN_ARGS_CNT        3
#define CNT_SIZE_BYTES      4
#define STRING_MAX_LEN      59
#define STRING_RECORD_LEN   (STRING_MAX_LEN + 1)
#define RECORD_LEN          (STRING_RECORD_LEN + CNT_SIZE_BYTES)
#define BITS_IN_BYTE        8


enum {
    ARGS_INDX_PROG_NAME,
    ARGS_INDX_FILE_NAME,
    ARGS_INDX_CMD,
    ARGS_INDX_ID,

    ARGS_INDX_NUM,
};

enum {
    CMD_INDX_ADD,
    CMD_INDX_QUARY,
    CMD_INDX_LIST,

    CMD_INDX_NUM
};

struct args_info {
    int fd;
    const char *file_name;
    const char *cmd;
    int cmd_id;
    const char *item;
};

const char * cmd_strings[] = {
    [CMD_INDX_ADD] = "add",
    [CMD_INDX_QUARY] = "quary",
    [CMD_INDX_LIST] = "list",
};

static void fill_args_info(struct args_info ai);

int main(int argc, char **argv)
{
    if (argc < MIN_ARGS_CNT) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }

    struct args_info ainfo;
    
    /* open file (text or binary)*/
    ainfo.file_name = argv[ARGS_INDX_FILE_NAME];
    ainfo.fd = open(ainfo.file_name, O_RDWR|O_CREAT);
    if (ainfo.fd == SYSCALL_OPEN_ERR) {
        perror(ainfo.file_name);
        return 1;
    }

    /* get key */
    /*    if (sscanf(argv[ARGS_INDX_KEY], "%u", &ainfo.key) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_KEY]);
        return 1;
        }*/




    close(ainfo.fd);
    
    return 0;
}


