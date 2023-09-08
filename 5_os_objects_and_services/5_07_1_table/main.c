/*
 * Task 5.07.1 (2.55)
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
#include "fstd.h"

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

/* enumerations ------------------------------------------------------------- */
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
    ERR_INDX_TOO_FEW_ARGS_FOR,
    ERR_INDX_TOO_LONG_ID,
};

enum status {
    STATUS_OK,
    STATUS_ERROR,
    STATUS_RECORD_UPSENT,
    STATUS_WRONG_POSITION,
    STATUS_LSEEK_ERROR,
    STATUS_READ_ERROR,
    STATUS_WRITE_ERROR,
};

/* structures --------------------------------------------------------------- */
struct file_info {
n    int fd;
    const char *filename;
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

/* local constants ---------------------------------------------------------- */
static const char * cmd_strings[] = {
    [CMD_INDX_ADD] = "add",
    [CMD_INDX_QUARY] = "quary",
    [CMD_INDX_LIST] = "list",
};

static const char * err_strings[] = {
    [ERR_INDX_WRONG_CMD] = "wrong command",
    [ERR_INDX_NO_ID] = "no id",
    [ERR_INDX_TOO_FEW_ARGS] = "too few arguments",
    [ERR_INDX_TOO_FEW_ARGS_FOR] = "too few arguments for the command",
    [ERR_INDX_TOO_LONG_ID] = "too long id",
};

/* local function declarations ---------------------------------------------- */
//static void fill_args_info(struct args_info ai);
static const char * get_cmd_string(const char * str);
static enum cmd_indx get_cmd_indx(const char * str);

/*
 * Searches a record pointed by id.
 *
 * @return Position id in designed file by fd.
 *         Non-negative position means the record exist.
 *         (-1) means the record is upsent.
 */
static off_t search_record(struct file_info *fi, const char *id);

/*
 * Write struct record to the file.
 *
 * @return Position writed struct record. 
 */
static enum status write_record(struct file_info *fi, const struct record *rec,
                                off_t rec_pos);

/*
 * Read record from desined position. 
 *
 * @return Position next record.
 */
static enum status read_record(struct file_info *fi, struct record *rec,
                               off_t rec_pos);

/*
 * Create record in the end of file.
 *
 * @return Position created record.
 */
static off_t create_record(struct file_info *fi, const char *id);

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

static enum status read_record(struct file_info *fi, struct record *rec,
                               off_t rec_pos)
{
    int remainder = rec_pos % sizeof(struct record);
    if (remainder != 0)
        return STATUS_WRONG_POSITION; 
    off_t res_pos = lseek(fi->fd, rec_pos, SEEK_SET);
    if (res_pos != rec_pos) {
        perror(fi->filename);
        return STATUS_LSEEK_ERROR;
    }
    int res_rd = fstd_file_read(fi->fd, fi->filename,
                               (char *)rec, sizeof(struct record));
    if (res_rd != sizeof(struct record)
        return STATUS_READ_ERROR;

    return STUTUS_OK;
}

static enum status write_record(struct file_info *fi, const struct record *rec,
                                off_t rec_pos)
{
    int remainder = rec_pos % sizeof(struct record);
    if (remainder != 0)
        return STATUS_WRONG_POSITION; 
    off_t res_pos = lseek(fi->fd, rec_pos, SEEK_SET);
    if (res_pos != rec_pos) {
        perror(fi->filename);
        return STATUS_LSEEK_ERROR;
    }
    int res_wr = fstd_file_write(fi->fd, fi->filename,
                                (const char *)rec, sizeof(struct record));
    if (res_wr != sizeof(struct record)
        return STATUS_WRITE_ERROR;
    return STATUS_OK;
}

static off_t search_record(struct file_info *fi, const char *id)
{
    if (string_length(id) > STRING_MAX_LEN) {
        fprintf(stderr, "%s: %s\n", err_strings[ERR_INDX_TOO_LONG_ID], id);
        return (-1);
    }
    off_t file_size = lseek(fi->fd, 0, SEEK_END);
    if (file_size == SYSCALL_LSEEK_ERR) {
        perror(fi->filename);
        return (-1);
    }
    struct record rec;
    off_t pos = 0;
    while((pos = read_record(fi, &rec, pos)) != file_size) {
        if (pos < 0)
            return pos;
        if (string_compare(id, rec.id))
            return (pos - sizeof(struct record));
    }
    return (-1);
}

static off_t create_record(struct file_info *fi, const char *id)
{
    if (string_length(id) > STRING_MAX_LEN) {
        fprintf(stderr, "%s: %s\n", err_strings[ERR_INDX_TOO_LONG_ID], id);
        return (-1);
    }

    struct record rec;
    strstd_copy(rec.id, STRING_RECORD_LEN, id);
    rec.cnt = 1;

    off_t file_size = lseek(fi->fd, 0, SEEK_END);
    if (file_size == SYSCALL_LSEEK_ERR) {
        perror(fi->filename);
        return (-1);
    }
    off_t res_wr = write_record(fi, &rec, file_size);

    return res_wr;
}

static void cmd_add(struct file_info *fi, const char *id)
{
    off_t pos = search_record(fi, id);
    if (pos != -1) {
        increase_record(fi, pos);
    } else {
        create_record(fi, id);
    }
}

static void cmd_quary(struct file_info *fi, const char *id)
{
    off_t pos = search_record(fi, id);
    if (pos != -1) {
        struct record rec;
        read_record(fi, &rec, pos);
        printf("%u\n", rec.cnt);
    } else {
        printf("0\n");
    }
}

static void cmd_list(struct file_info *fi, const char *id)
{

}
