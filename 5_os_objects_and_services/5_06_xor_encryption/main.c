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
};

static void encrypt(struct args_info *ai);
static void encrypt_xor(unsigned char *buf, size_t buf_size, unsigned key);

int main(int argc, char **argv)
{
    if (argc < MIN_ARGS_CNT) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }

    struct args_info ainfo;
    
    /* open file (text or binary)*/
    char *file_name = argv[ARGS_INDX_FILE_NAME];
    ainfo.fd = open(file_name, O_RDWR);
    if (ainfo.fd == SYSCALL_OPEN_ERR) {
        perror(file_name);
        return 1;
    }

    /* get key */
    if (sscanf(argv[ARGS_INDX_KEY], "%u", &ainfo.key) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_KEY]);
        return 1;
    }

    unsigned char buf[] = {
        1, 2, 3, 4, 5, 6, 7
    };
    encrypt_xor(buf, sizeof(buf), 0xAABBCCDD);
    
    close(ainfo.fd);
    
    return 0;
}

static void encrypt(struct args_info *ai)
{
    // long file_size = lseek(ai->fd, 0, SEEK_END);
    // size_t block_cnt = file_size / RDWR_BLOCK_SIZE;

    // unsigned char buf[RDWR_BLOCK_SIZE];
    // int rd;
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
        unsigned char *num = &buf[buf_size - 1 - i];
        unsigned char key_byte = (unsigned char)(key >> (i * BITS_IN_BYTE));
        *num ^= key_byte;
    }
}