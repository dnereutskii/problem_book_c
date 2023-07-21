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

#define RDWR_BLOCK_SIZE     4096
#define MIN_ARGS_CNT        3
#define KEY_SIZE_BYTES      4
#define BITS_IN_BYTE        8
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
    const char *file_name;
};

static void encrypt_file(struct args_info *ai);
static void encrypt_file2(struct args_info *ai);
static void encrypt_xor(unsigned char *buf, size_t buf_size, unsigned key);

int main(int argc, char **argv)
{
    if (argc < MIN_ARGS_CNT) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }

    struct args_info ainfo;
    
    /* open file (text or binary)*/
    ainfo.file_name = argv[ARGS_INDX_FILE_NAME];
    ainfo.fd = open(ainfo.file_name, O_RDWR);
    if (ainfo.fd == SYSCALL_OPEN_ERR) {
        perror(ainfo.file_name);
        return 1;
    }

    /* get key */
    if (sscanf(argv[ARGS_INDX_KEY], "%u", &ainfo.key) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_KEY]);
        return 1;
    }

    // unsigned char buf[] = {
        // 1, 2, 3, 4, 5, 6, 7
    // };
    // encrypt_xor(buf, sizeof(buf), 0xAABBCCDD);
    // encrypt_xor(buf, sizeof(buf), 0xAABBCCDD);
    
    // encrypt_file(&ainfo);
    encrypt_file2(&ainfo);
    

    close(ainfo.fd);
    
    return 0;
}

static void encrypt_file(struct args_info *ai)
{
    unsigned char buf[RDWR_BLOCK_SIZE];
    long file_size = lseek(ai->fd, 0, SEEK_END);
    int wr, rd;
    size_t block_cnt = file_size / RDWR_BLOCK_SIZE;
    size_t rest_cnt = file_size % RDWR_BLOCK_SIZE;
    lseek(ai->fd, 0, SEEK_SET);
    /* block handler */
    for (size_t i = 0; i < block_cnt; i++) {
        /* read block */
        rd = 0;
        while (rd != RDWR_BLOCK_SIZE) {
            int res = read(ai->fd, &buf[rd], RDWR_BLOCK_SIZE - rd);
            if (res == SYSCALL_READ_ERR) {
                perror(ai->file_name);
                return;
            }
            rd += res;
        }
        encrypt_xor(buf, RDWR_BLOCK_SIZE, ai->key);
        /* write block */
        lseek(ai->fd, i * RDWR_BLOCK_SIZE, SEEK_SET);
        wr = 0;
        while (wr != RDWR_BLOCK_SIZE) {
            int res = write(ai->fd, &buf[wr], RDWR_BLOCK_SIZE - wr);
            if (res == SYSCALL_WRITE_ERR) {
                perror(ai->file_name);
                return;
            }
            wr += res;
        }
    }
    /* remainder handler */
    rd = 0;
    while (rd != rest_cnt) {
        int res = read(ai->fd, &buf[rd], rest_cnt - rd);
        if (res == SYSCALL_READ_ERR) {
            fprintf(stderr, "read error\n");
            return;
        }
        rd += res;
    }
    encrypt_xor(buf, rest_cnt, ai->key);
    lseek(ai->fd, -rest_cnt, SEEK_END);
    wr = 0;
    while (wr != rest_cnt) {
        int res = write(ai->fd, &buf[wr], rest_cnt - wr);
        if (res == SYSCALL_WRITE_ERR) {
            fprintf(stderr, "write error\n");
            return;
        }
        wr += res;
    }
}

static void encrypt_file2(struct args_info *ai)
{
    long file_size = lseek(ai->fd, 0, SEEK_END);
    unsigned char buf[RDWR_BLOCK_SIZE];
    int wr, rd, rest;
    long pos = lseek(ai->fd, 0, SEEK_SET);
    //long pos_wr = pos_rd;

    while((rd = read(ai->fd, buf, RDWR_BLOCK_SIZE)) != SYSCALL_READ_EOF) {
        if (rd == SYSCALL_READ_ERR) {
            perror(ai->file_name);
            return;
        }
        pos = lseek(ai->fd, 0, SEEK_CUR); /* save rd pos */
        if (pos != file_size) {
            rest = rd % BITS_IN_BYTE;
            if (rest) {
                rd -= rest; /* after this may be 0 */
                pos = lseek(ai->fd, -rest, SEEK_CUR); /* save&set rd pos */
            }
        }
        if (rd == 0) /* pass 0 */
            continue;
        encrypt_xor(buf, rd, ai->key);
        wr = 0;
        lseek(ai->fd, -rd, SEEK_CUR); /* set wr pos */
        while (wr != rd) {
            int res = write(ai->fd, &buf[wr], rd - wr);
            if (res == SYSCALL_WRITE_ERR) {
                perror(ai->file_name);
                return;
            }
            wr += res;
        }
        //pos_wr = lseek(ai->fd, 0, SEEK_CUR); /* save wr pos */
        //lseek(ai->fd, pos_rd, SEEK_SET); /* set rd pod */
    }
}

static void encrypt_xor(unsigned char *buf, size_t buf_size, unsigned key)
{
    size_t four_byte_cnt = buf_size / KEY_SIZE_BYTES;
    size_t rest_cnt = buf_size % KEY_SIZE_BYTES;
    /* four-byte number handler */
    for (size_t i = 0; i < four_byte_cnt; i++) {
        unsigned *num = (unsigned *)&buf[i * KEY_SIZE_BYTES];
        *num ^= key;
    }
    /* rest bytes handler */
    for (size_t i = 0; i < rest_cnt; i++) {
        unsigned char *num = &buf[buf_size - rest_cnt + i];
        unsigned char key_byte = (unsigned char)(key >> (i * BITS_IN_BYTE));
        *num ^= key_byte;
    }
}
