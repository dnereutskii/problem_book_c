#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include "fstd.h"

void fstd_fill_array(unsigned char *array, size_t len, unsigned char ch)
{
    while (len) {
        *array = ch;
        array++;
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
    
    int rd = 0;
    while (rd != buff_size) {
        int res = read(fd, &buff[rd], buff_size - rd);
        if (res < 0) {
            perror(filename);
            return res;
        }
        rd += res;
    }
    return rd;
}
