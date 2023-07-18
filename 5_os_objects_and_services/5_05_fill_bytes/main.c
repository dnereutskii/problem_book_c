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

#define WRITE_BLOCK_SIZE    1024
#define MIN_ARGS_CNT        5
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
    unsigned pos;
    unsigned len;
    unsigned byte;
};

int main(int argc, char **argv)
{
    if (argc < MIN_ARGS_CNT) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }

    args_info ainfo;
    
    char *file_name = argv[ARGS_INDX_FILE_NAME];
    ainfo.fd = open(file_name, O_WRONLY);
    if (ainfo.fd == SYSCALL_OPEN_ERR) {
        perror(file_name);
        return 1;
    }

    if (sscanf(argv[ARGS_INDX_POS], "%u", &ainfo.pos) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_POS]);
        // perror(argv[ARGS_INDX_POS]);
        return 1;
    }

    if (sscanf(argv[ARGS_INDX_LEN], "%u", &ainfo.len) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_LEN]);
        // perror(argv[ARGS_INDX_LEN]);
        return 1;
    }

    if (sscanf(argv[ARGS_INDX_BYTE], "%u", &ainfo.byte) != 1) {
        fprintf(stderr, "error handling %s\n", argv[ARGS_INDX_BYTE]);
        // perror(argv[ARGS_INDX_BYTE]);
        return 1;
    }
    if (ainfo.byte > 255) {
        fprintf(stderr, "byte exceed 255\n");
        return 1;
    }

    printf("%s %u %u %u", ainfo.file_name, ainfo.pos, ainfo.len, ainfo.byte);

    fill_bytes(&ainfo);
    
    close(file);
    
    return 0;
}

static void fill_bytes(struct args_info * ai)
{
    long size = lseek(ai->fd, 0, SEEK_END);
    if (ai->pos > size) {
        fprintf(stderr, "pos %lu greter than file size %ls\n", ai.pos, size);
        return;
    }
    size  = lseek(fd, ai->pos, SEEK_SET); 
    if (ai->len < WRITE_BLOCK_SIZE) {
        write(ai->fd, &ai->byte, 1);
    } else {
        unsigned char arr[WRITE_BLOCK_SIZE];
        fill_array(arr, WRITE_BLOCK_SIZE, (unsigned char)ai->byte);
        for (size_t i = 0; i < ai->len / WRITE_BLOCK_SIZE; i++) {
            int res = write(ai->fd, &arr, WRITE_BLOCK_SIZE);
            if (res == SYSCALL_WRITE_ERR) {
                fprintf(stderr, "write error\n");
                return;
            }
        }
        write(ai->fd, &arr, ai->len % WRITE_BLOCK_SIZE);
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
