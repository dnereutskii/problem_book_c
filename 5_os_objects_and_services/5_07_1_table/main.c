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

#define _LARGEFILE64_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include "strstd.h"

/* SYSCALLS defines */
#define SYSCALL_OPEN_ERR    (-1)
#define SYSCALL_READ_EOF    (0)
#define SYSCALL_READ_ERR    (-1)
#define SYSCALL_WRITE_ERR   (-1)
#define SYSCALL_LSEEK_ERR   (-1)

/* USER defines */
#define RDWR_BLOCK_SIZE     4096
#define ARGS_MIN_CNT        3
#define ARGS_CNT_ADD_QUARY  4
#define ARGS_CNT_LIST       3
#define CNT_SIZE_BYTES      4
#define STRING_MAX_LEN      59
#define STRING_RECORD_LEN   (STRING_MAX_LEN + 1)
#define RECORD_LEN          (STRING_RECORD_LEN + CNT_SIZE_BYTES)
#define BITS_IN_BYTE        8


enum args_indx {
    ARGS_INDX_PROG_NAME,
    ARGS_INDX_FILE_NAME,
    ARGS_INDX_CMD,
    ARGS_INDX_ID,

    ARGS_INDX_NUM,
};

enum cmd_indx {
    CMD_INDX_ADD,
    CMD_INDX_QUARY,
    CMD_INDX_LIST,

    CMD_INDX_NUM,
    CMD_INDX_ERR = -1,
};

enum err_indx {
    ERR_INDX_WRONG_CMD,
    ERR_INDX_NO_ID,
    ERR_INDX_TOO_FEW_ARGS,
    ERR_INDX_TOO_FEW_ARGS_FOR
};

struct file_info {
    int fd;
    const char *file_name;
}

struct args_info {
    struct file_info fi;
    const char *cmd;
    enum cmd_indx cmd_id;
    const char *item;
};

struct __attribute__((__packed__)) record {
    char id[STRING_RECORD_LEN];
    unsigned cnt;
}

const char * cmd_strings[] = {
    [CMD_INDX_ADD] = "add",
    [CMD_INDX_QUARY] = "quary",
    [CMD_INDX_LIST] = "list",
};

const char * err_strings[] = {
    [ERR_INDX_WRONG_CMD] = "wrong command",
    [ERR_INDX_NO_ID] = "no id",
    [ERR_INDX_TOO_FEW_ARGS] = "too few arguments",
    [ERR_INDX_TOO_FEW_ARGS_FOR] = "too few arguments for the command"
};

//static void fill_args_info(struct args_info ai);
static const char * get_cmd_string(const char * str);
static enum cmd_indx get_cmd_indx(const char * str);

/*
 * Searches a record pointed by id.
 *
 * @return Position id in designed file by fd.
 *         Positive position means the record exist.
 *         -1 means the record is upsent.
 */
static off_t search_record(int fd, const char *id);

/*
 * Write struct record to the file.
 *
 * @return Position writed struct record. 
 */
static off_t write_record(int fd, off_t pos, const struct record *req);

/*
 * Read record from desined position. 
 *
 * @return Position next record.
 */
static off_t read_record(int fd, off_t pos, struct record *req);

/*
 * Writes bytes to the file from the position.
 *
 * @return Position next after writed bytes.
 */
static off_t write_buff(int fd, off_t pos, const char *buff, size_t buff_size);

/*
 * Read bytes from the file starting the position.
 *
 * @return Position next after readed buff_size.
 */
static off_t read_buff(int fd, off_t pos, char *buff, size_t buff_size);

int main(int argc, char **argv)
{
    /* check min args count */
    if (argc < ARGS_MIN_CNT) {
        fprintf(stderr, "%s\n", err_strings[ERR_INDX_TOO_FEW_ARGS]);
        return 1;
    }
    
    struct args_info ainfo;

    /* check command */
    ainfo.cmd_id = get_cmd_indx(argv[ARGS_INDX_CMD]);
    if (ainfo.cmd_id == CMD_INDX_ERR) {
        fprintf(stderr, "%s: %s\n", argv[ARGS_INDX_CMD],
                err_strings[ERR_INDX_WRONG_CMD]);
        return 1;
    }

    /* check args count depending on the command */
    switch (ainfo.cmd_id) {
    case CMD_INDX_ADD:
    case CMD_INDX_QUARY:
        if (argc < ARGS_CNT_ADD_QUARY) {
            fprintf(stderr, "%s: %s\n", argv[ARGS_INDX_CMD],
                    err_strings[ERR_INDX_TOO_FEW_ARGS_FOR]);
            return 1;
        }
        break;
    default:
        break;
    }
    
    /* open file (text or binary)*/
    ainfo.file_name = argv[ARGS_INDX_FILE_NAME];
    ainfo.fd = open(ainfo.file_name, O_RDWR|O_CREAT, 0666);
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

static const char * get_cmd_string(const char * str)
{
    for (size_t i = 0; i < CMD_INDX_NUM; i++) {
        bool res = string_compare(cmd_strings[i], str);
        if (res)
            return cmd_strings[i];
    }

    return NULL;
}

static enum cmd_indx get_cmd_indx(const char * str)
{
    for (size_t i = 0; i < CMD_INDX_NUM; i++) {
        bool res = string_compare(cmd_strings[i], str);
        if (res)
            return (enum cmd_indx)i;
    }

    return CMD_INDX_ERR;
}

static off_t write_buff(struct file_info *fi, off_t pos, const char *buff,
                        size_t buff_size)
{
    size_t wr = 0;
    pos = lseek(fd, pos, SEEK_SET); /* set wr pos */
    if (pos == SYSCALL_LSEEK_ERR) {
        perror(fi->file_name);
        return pos;
    }
    while (wr != buff_size) {
        int res = write(fi->fd, &buf[wr], buff_size - wr);
        if (res == SYSCALL_WRITE_ERR) {
            perror(fi->file_name);
            return res;
        }
        wr += res;
    }
}
