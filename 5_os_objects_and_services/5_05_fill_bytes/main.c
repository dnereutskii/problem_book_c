/*
 * Task 5.05
 *
 * ./prog test.txt pos len byte
 *
 * @author rhetti
 * @date 07.2023
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#define WRITE_BLOCK_SIZE    16
#define MIN_ARGS_CNT        5
#define BYTE_MAX_VALUE      255    
#define SYSCALL_OPEN_ERR    (-1)
#define SYSCALL_READ_EOF    (0)
#define SYSCALL_READ_ERR    (-1)
#define SYSCALL_WRITE_ERR   (-1)

enum {
    ARGS_INDX_FILE_NAME = 1,
    ARGS_INDX_POS,
    ARGS_INDX_LEN,
    ARGS_INDX_BYTE,
};

struct args_info {
    int fd;
    size_t pos;
    size_t len;
    unsigned byte;
};

static void fill_bytes(struct args_info *ai);
static void fill_array(unsigned char *array, size_t len, unsigned char ch);

int main(int argc, char **argv)
{
    if (argc < MIN_ARGS_CNT) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }

    struct args_info ainfo;
    
    char *file_name = argv[ARGS_INDX_FILE_NAME];
    ainfo.fd = open(file_name, O_WRONLY);
    if (ainfo.fd == SYSCALL_OPEN_ERR) {
        perror(file_name);
        return 1;
    }

    if (sscanf(argv[ARGS_INDX_POS], "%u", &ainfo.pos) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_POS]);
        return 1;
    }

    if (sscanf(argv[ARGS_INDX_LEN], "%u", &ainfo.len) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_LEN]);
        return 1;
    }

    if (sscanf(argv[ARGS_INDX_BYTE], "%u", &ainfo.byte) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_BYTE]);
        return 1;
    }
    if (ainfo.byte > BYTE_MAX_VALUE) {
        fprintf(stderr, "byte exceed 255\n");
        return 1;
    }

    // printf("%s %u %u %u\n", file_name, ainfo.pos, ainfo.len, ainfo.byte);

    fill_bytes(&ainfo);
    
    close(ainfo.fd);
    
    return 0;
}

static void fill_bytes(struct args_info *ai)
{
    lseek(ai->fd, ai->pos, SEEK_SET);
    if (ai->len < WRITE_BLOCK_SIZE) {
        size_t len_copy = ai->len;
        while (len_copy) {
            int wr = write(ai->fd, &ai->byte, 1);
            if (wr == SYSCALL_WRITE_ERR) {
                fprintf(stderr, "write error\n");
                return;
            }
            len_copy--;
        }
    } else {
        unsigned char arr[WRITE_BLOCK_SIZE];
        int wr;
        fill_array(arr, WRITE_BLOCK_SIZE, (unsigned char)ai->byte);
        /* quotient of writing */
        for (size_t i = 0; i < (ai->len / WRITE_BLOCK_SIZE); i++) {
            wr = 0;
            while (wr != WRITE_BLOCK_SIZE) {
                int res = write(ai->fd, &arr[wr], WRITE_BLOCK_SIZE - wr);
                if (res == SYSCALL_WRITE_ERR) {
                    fprintf(stderr, "write error\n");
                    return;
                }
                wr += res;
            }
        }
        /* remainder of writing */
        wr = 0;
        while (wr != (ai->len % WRITE_BLOCK_SIZE)) {
            int res = write(ai->fd, &arr[wr], ai->len % WRITE_BLOCK_SIZE - wr);
            if (res == SYSCALL_WRITE_ERR) {
                fprintf(stderr, "write error\n");
                return;
            }
            wr += res;
        }

    }
}

static void fill_array(unsigned char *array, size_t len, unsigned char ch)
{
    while (len) {
        *array = ch;
        array++;
        len--;
    }
}
