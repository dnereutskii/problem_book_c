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
#define ARGS_MIN_CNT        4
#define CNT_SIZE_BYTES      4
#define STRING_MAX_LEN      59
#define STRING_RECORD_LEN   (STRING_MAX_LEN + 1)
#define RECORD_LEN          (STRING_RECORD_LEN + CNT_SIZE_BYTES)

/* enumerations ------------------------------------------------------------- */
enum args_indx {
    ARGS_INDX_PROG_NAME,
    ARGS_INDX_TABLE_1_NAME,
    ARGS_INDX_TABLE_2_NAME,
    ARGS_INDX_TABLE_PIVOT_NAME,

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
static const char * err_strings[] = {
    [ERR_INDX_WRONG_CMD] = "wrong command",
    [ERR_INDX_NO_ID] = "no id",
    [ERR_INDX_TOO_FEW_ARGS] = "too few arguments",
    [ERR_INDX_TOO_FEW_ARGS_FOR] = "too few arguments for the command",
    [ERR_INDX_TOO_LONG_ID] = "too long id",
};

/* local function declarations ---------------------------------------------- */

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
 * Read record from desined position. 
 *
 * @return Operation status.
 */
static enum status read_record(const struct file_info *fi, struct record *rec,
                               off_t rec_pos);

/**
 * Check indentificator length
 *
 * @return STATUS_OK 
 * @return STATUS_ERR_TOO_LONG_ID
 */
static enum status check_id_len(const char *id);

/**
 * Search for equal records in two files and writes to another file. 
 *
 * @param t1 Record table 1
 * @param t2 Record table 2
 * @param tp Record table pivot
 * @param sum Summing flag
 *
 */
static void find_equels(const struct file_info *t1, const struct file_info *t2,
                        const struct file_info *tp, bool sum);

int main(int argc, char **argv)
{
    /* check min args count */
    if (argc < ARGS_INDX_NUM) {
        fprintf(stderr, "%s\n", err_strings[ERR_INDX_TOO_FEW_ARGS]);
        return 1;
    }
    
    struct file_info table_1;
    struct file_info table_2;
    struct file_info table_pivot;
    
    /* open file (table 1)*/
    table_1.filename = argv[ARGS_INDX_TABLE_1_NAME];
    table_1.fd = open(table_1.filename, O_RDONLY);
    if (table_1.fd == SYSCALL_OPEN_ERR) {
        perror(table_1.filename);
        return 1;
    }

    /* open file (table 2)*/
    table_2.filename = argv[ARGS_INDX_TABLE_2_NAME];
    table_2.fd = open(table_2.filename, O_RDONLY);
    if (table_2.fd == SYSCALL_OPEN_ERR) {
        perror(table_2.filename);
        return 1;
    }

    /* open file (table pivot)*/
    table_pivot.filename = argv[ARGS_INDX_TABLE_PIVOT_NAME];
    table_pivot.fd = open(table_pivot.filename, O_RDWR|O_CREAT|O_TRUNC|O_APPEND,
                          0666);
    if (table_pivot.fd == SYSCALL_OPEN_ERR) {
        perror(table_pivot.filename);
        return 1;
    }

    find_equels(&table_1, &table_2, &table_pivot, true);
    find_equels(&table_2, &table_pivot, &table_pivot, false);

    close(table_1.fd);
    close(table_2.fd);
    close(table_pivot.fd);
    
    return 0;
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

static void find_equels(const struct file_info *t1, const struct file_info *t2,
                        const struct file_info *tp, bool sum)
{
    assert(t1 != NULL);
    assert(t2 != NULL);
    assert(tp != NULL);

    off_t file_size_t1 = lseek(t1->fd, 0, SEEK_END);
    if (file_size_t1 == SYSCALL_LSEEK_ERR) {
        perror(t1->filename);
        return;
    }

    if (lseek(t1->fd, 0, SEEK_SET) == SYSCALL_LSEEK_ERR) {/* from start */
        perror(t1->filename);
        return;
    }
    
    for (size_t i = 0; i < file_size_t1; i += sizeof(struct record)) {
        struct record rec_t1 = {0};
        off_t rec_pos;
        unsigned rec_cnt;
        if (read_record(t1, &rec_t1, i) != STATUS_OK)
            return;
        enum status exist = search_record(t2, rec_t1.id, &rec_pos);
        if (exist == STATUS_OK) {
            if (sum) {
                get_record_cnt(t2, rec_pos, &rec_cnt);
                rec_t1.cnt += rec_cnt;
            } else {
                continue;
            }
        }
        fstd_file_write(tp->fd, tp->filename, (const char *)&rec_t1,
                        sizeof(struct record));
    }
}
