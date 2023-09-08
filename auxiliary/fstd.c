#include <stdio.h>
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
    int written = 0;
    while (written != buff_size) {
        int res = write(fd, &buf[written], buff_size - written);
        if (res < 0) {
            perror(filename);
            return res;
        }
        written += res;
    }
    return written;
}

int fstd_file_read(int fd, const char *filename, char *buff, size_t buff_size)
{
    int read = 0;
    while (read != buff_size) {
        int res = read(fd, &buf[read], buff_size - read);
        if (res < 0) {
            perror(filename);
            return res;
        }
        read += res;
    }
    return read;
}