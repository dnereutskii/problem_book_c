/*
 * @brief Task 5.07.2 (2.56)
 * 
 * The program creates pivot table from 2 others tables.
 * 
 * Call string: ./pivot table1 table2 pivot_table
 *
 * @author rhetti
 * @date 09.2023
 *
 */

#define _LARGEFILE64_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
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
    STATUS_RECORD_UPSENT,
    /* ERRORS */
    STATUS_ERR_WRONG_POSITION,
    STATUS_ERR_LSEEK,
    STATUS_ERR_READ,
    STATUS_ERR_WRITE,
    STATUS_ERR_TOO_LONG_ID,
};

/* structures --------------------------------------------------------------- */
struct file_info {
    int fd;
    const char *filename;
};

struct args_info {
    struct file_info fi;
    const char *cmd;
    enum cmd_indx cmd_id;
    const char *item;
};

struct __attribute__((__packed__)) record {
    char id[STRING_RECORD_LEN];
    unsigned cnt;                /* 4 bytes */
};

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
/*static const char * get_cmd_string(const char * str);*/
static enum cmd_indx get_cmd_indx(const char * str);

/**
 * Searches a record pointed by id.
 *
 * @return Position id in designed file by fd.
 *         Non-negative position means the record exist.
 *         (-1) means the record is upsent.
 */
static enum status search_record(const struct file_info *fi, const char *id,
                                 off_t *rec_pos);

/**
 * Write record to the designed position.
 *
 * @return Operation status. 
 */
static enum status write_record(const struct file_info *fi, const struct record *rec,
                                off_t rec_pos);

/**
 * Read record from desined position. 
 *
 * @return Operation status.
 */
static enum status read_record(const struct file_info *fi, struct record *rec,
                               off_t rec_pos);

/**
 * Create record in the end of file.
 *
 * @return Position created record.
 */
static enum status create_record(const struct file_info *fi, const char *id);

/**
 * Check indentificator length
 *
 * @return STATUS_OK 
 * @return STATUS_ERR_TOO_LONG_ID
 */
static enum status check_id_len(const char *id);

static void cmd_add(const struct file_info *fi, const char *id);

static void cmd_quary(const struct file_info *fi, const char *id);

static void cmd_list(const struct file_info *fi);

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
    ainfo.fi.filename = argv[ARGS_INDX_FILE_NAME];
    ainfo.fi.fd = open(ainfo.fi.filename, O_RDWR|O_CREAT, 0666);
    if (ainfo.fi.fd == SYSCALL_OPEN_ERR) {
        perror(ainfo.fi.filename);
        return 1;
    }

    switch (ainfo.cmd_id) {
    case CMD_INDX_ADD:
        cmd_add(&ainfo.fi,  argv[ARGS_INDX_ID]);
        break;
    case CMD_INDX_QUARY:
        cmd_quary(&ainfo.fi,  argv[ARGS_INDX_ID]);
        break;
    case CMD_INDX_LIST:
        cmd_list(&ainfo.fi);
        break;
    default:
        break;
    }

    close(ainfo.fi.fd);
    
    return 0;
}

#if 0 /* not used */
static const char * get_cmd_string(const char * str)
{
    for (size_t i = 0; i < CMD_INDX_NUM; i++) {
        bool res = strstd_compare(cmd_strings[i], str);
        if (res)
            return cmd_strings[i];
    }

    return NULL;
}
#endif

static enum cmd_indx get_cmd_indx(const char * str)
{
    for (size_t i = 0; i < CMD_INDX_NUM; i++) {
        bool res = strstd_compare(cmd_strings[i], str);
        if (res)
            return (enum cmd_indx)i;
    }

    return CMD_INDX_ERR;
}

static enum status read_record(const struct file_info *fi, struct record *rec,
                               off_t rec_pos)
{
    int remainder = rec_pos % sizeof(struct record);
    if (remainder != 0)
        return STATUS_ERR_WRONG_POSITION;
    off_t cur_pos = lseek(fi->fd, rec_pos, SEEK_SET);
    if (cur_pos == SYSCALL_LSEEK_ERR) {
        perror(fi->filename);
        return STATUS_ERR_LSEEK;
    }
    int res_rd = fstd_file_read(fi->fd, fi->filename,
                               (char *)rec, sizeof(struct record));
    if (res_rd != sizeof(struct record))
        return STATUS_ERR_READ;

    return STATUS_OK;
}

static enum status write_record(const struct file_info *fi,
                                const struct record *rec, off_t rec_pos)
{
    assert(fi != NULL);
    
    int remainder = rec_pos % sizeof(struct record);
    if (remainder != 0)
        return STATUS_ERR_WRONG_POSITION;

    off_t cur_pos = lseek(fi->fd, rec_pos, SEEK_SET);
    if (cur_pos == SYSCALL_LSEEK_ERR) {
        perror(fi->filename);
        return STATUS_ERR_LSEEK;
    }

    int res_wr = fstd_file_write(fi->fd, fi->filename,
                                (const char *)rec, sizeof(struct record));
    if (res_wr != sizeof(struct record))
        return STATUS_ERR_WRITE;
    return STATUS_OK;
}

static enum status search_record(const struct file_info *fi, const char *id,
                                 off_t *rec_pos)
{
    assert(fi != NULL);
    assert(rec_pos != NULL);

    enum status res = check_id_len(id);
    if (res != STATUS_OK)
        return res;
    
    off_t file_size = lseek(fi->fd, 0, SEEK_END);
    if (file_size == SYSCALL_LSEEK_ERR) {
        perror(fi->filename);
        return STATUS_ERR_LSEEK;
    }
    
    off_t cur_pos = lseek(fi->fd, 0, SEEK_SET); /* from start */
    if (cur_pos == SYSCALL_LSEEK_ERR) {
        perror(fi->filename);
        return STATUS_ERR_LSEEK;
    }
    
    struct record rec;
    for (size_t i = 0; i < file_size; i += sizeof(struct record)) {
        res = read_record(fi, &rec, i);
        if (res != STATUS_OK)
            return res;
        if (strstd_compare(id, rec.id)) {
            *rec_pos = i;
            return STATUS_OK;
        }
    }
    
    return STATUS_RECORD_UPSENT;
}

static enum status create_record(const struct file_info *fi, const char *id)
{
    assert(fi != NULL);

    enum status res = check_id_len(id);
    if (res != STATUS_OK)
        return res;
    
    struct record rec = {0};
    strstd_copy(rec.id, STRING_RECORD_LEN, id);
    rec.cnt = 1;

    off_t file_size = lseek(fi->fd, 0, SEEK_END);
    if (file_size == SYSCALL_LSEEK_ERR) {
        perror(fi->filename);
        return STATUS_ERR_LSEEK;
    }
    
    return write_record(fi, &rec, file_size);
}

static enum status increase_record_cnt(const struct file_info *fi, off_t rec_pos)
{
    assert(fi != NULL);

    unsigned cnt;
    
    int remainder = rec_pos % sizeof(struct record);
    if (remainder != 0)
        return STATUS_ERR_WRONG_POSITION;
    
    off_t cur_pos = lseek(fi->fd, rec_pos + STRING_RECORD_LEN, SEEK_SET);
    if (cur_pos == SYSCALL_LSEEK_ERR) {
        perror(fi->filename);
        return STATUS_ERR_LSEEK;
    }
    
    int res = fstd_file_read(fi->fd, fi->filename, (char *)&cnt, sizeof(cnt));
    if (res != sizeof(cnt))
        return STATUS_ERR_READ;

    cnt += 1;

    cur_pos = lseek(fi->fd, -sizeof(cnt), SEEK_CUR);
    if (cur_pos == SYSCALL_LSEEK_ERR) {
        perror(fi->filename);
        return STATUS_ERR_LSEEK;
    }

    res = fstd_file_write(fi->fd, fi->filename, (const char *)&cnt, sizeof(cnt));
    if (res != sizeof(cnt))
        return STATUS_ERR_WRITE;

    return STATUS_OK;
}
    
static enum status get_record_cnt(const struct file_info *fi, off_t rec_pos,
                                  unsigned *cnt)
{
    assert(fi != NULL);

    int remainder = rec_pos % sizeof(struct record);
    if (remainder != 0)
        return STATUS_ERR_WRONG_POSITION;
    
    off_t cur_pos = lseek(fi->fd, rec_pos + STRING_RECORD_LEN, SEEK_SET);
    if (cur_pos == SYSCALL_LSEEK_ERR) {
        perror(fi->filename);
        return STATUS_ERR_LSEEK;
    }
    
    int res = fstd_file_read(fi->fd, fi->filename, (char *)cnt,
                             sizeof(unsigned));
    if (res != sizeof(unsigned))
        return STATUS_ERR_READ;

    return STATUS_OK;
}

static enum status check_id_len(const char *id)
{
    assert(id != NULL);
    if (strstd_length(id) > STRING_MAX_LEN) {
        fprintf(stderr, "%s: %s\n", err_strings[ERR_INDX_TOO_LONG_ID], id);
        return STATUS_ERR_TOO_LONG_ID;
    }
    return STATUS_OK;
}
 
static void cmd_add(const struct file_info *fi, const char *id)
{
    off_t pos;
    enum status res = search_record(fi, id, &pos);
    switch (res) {
    case STATUS_OK:
        increase_record_cnt(fi, pos);
        break;
    case STATUS_RECORD_UPSENT:
        create_record(fi, id);
        break;
    default:
        break;
    }
}

static void cmd_quary(const struct file_info *fi, const char *id)
{
    off_t rec_pos;
    unsigned rec_cnt;
    enum status res = search_record(fi, id, &rec_pos);
    switch (res) {
    case STATUS_OK:
        if (get_record_cnt(fi, rec_pos, &rec_cnt) == STATUS_OK)
            printf("%u\n", rec_cnt);
        break;
    case STATUS_RECORD_UPSENT:
        printf("0\n");
        break;
    default:
        break;
    }
}

static void cmd_list(const struct file_info *fi)
{
    assert(fi != NULL);
    
    off_t file_size = lseek(fi->fd, 0, SEEK_END);
    if (file_size == SYSCALL_LSEEK_ERR) {
        perror(fi->filename);
        return;
    }
    
    off_t cur_pos = lseek(fi->fd, 0, SEEK_SET); /* from start */
    if (cur_pos == SYSCALL_LSEEK_ERR) {
        perror(fi->filename);
        return;
    }
    
    struct record rec;
    for (size_t i = 0; i < file_size; i += sizeof(struct record)) {
        int rd_res = read_record(fi, &rec, i);
        if (rd_res != STATUS_OK)
            return;
        printf("%s %u\n", rec.id, rec.cnt);
    }
}

