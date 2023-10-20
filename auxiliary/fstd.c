#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include "fstd.h"

#define SYSCALL_READ_EOF (0)

void fstd_fill_array(unsigned char *array, size_t len, unsigned char ch)
{
    while (len) {
        *array = ch;
        array++;
        len--;
    }
}

void fstd_fill_arr_int(int *arr, size_t len, int num)
{
    while (len) {
        *arr = num;
        arr++;
        len--;
    }
}

int fstd_file_write(int fd, const char *filename,
                    const char *buff, size_t buff_size)
{
    assert(buff != NULL);
    
    int written = 0;
    while (written != buff_size) {
        int res = write(fd, &buff[written], buff_size - written);
        if (res < 0) {
            if (errno == EINTR || /**/
                errno == EAGAIN)  /**/
                continue;
            perror(filename);
            return res;
        }
        written += res;
    }
    return written;
}

int fstd_file_read(int fd, const char *filename, char *buff, size_t buff_size)
{
    assert(buff != NULL);
    
    int rc = 0;
    while (rc != buff_size) {
        int res = read(fd, &buff[rc], buff_size - rc);
        if (res < 0) {
            perror(filename);
            return res;
        }
        if (res == SYSCALL_READ_EOF)
            break;
        rc += res;
    }
    return rc;
}

