/*
 * Task 5.04
 *
 * The program counts lines in a text file, whose name is passing through the
 * command line. Reading by read syscall and by block 4096. Prints lines
 * counter by printf function.
 *
 * @author rhetti
 * @date 07.2023
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#define READ_BLOCK_SIZE     4096
#define MIN_ARGS_CNT        2
#define SYSCALL_OPEN_ERR    (-1)
#define SYSCALL_READ_EOF    (0)
#define SYSCALL_READ_ERR    (-1)

static size_t count_lines(int fd);
static size_t count_char(char *buf, size_t buf_size, char ch);

int main(int argc, char **argv)
{
    if (argc < MIN_ARGS_CNT) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }

    char *text_file_name = argv[1];
    int text_file = open(text_file_name, O_RDONLY);
    if (text_file == SYSCALL_OPEN_ERR) {
        perror(text_file_name);
        return 1;
    }

    size_t line_cnt = count_lines(text_file);
    printf("%u\n", line_cnt);

    close(text_file);
    
    return 0;
}

static size_t count_lines(int fd)
{
    ssize_t rd;
    size_t line_cnt = 0;
    char buf[READ_BLOCK_SIZE];

    while((rd = read(fd, buf, READ_BLOCK_SIZE)) != SYSCALL_READ_EOF) {
        line_cnt += count_char(buf, rd, '\n');
    }
    
    return line_cnt;
}

static size_t count_char(char *buf, size_t buf_size, char ch)
{
    size_t char_cnt = 0;

    while (buf_size) {
        if (*buf == ch)
            char_cnt++;
        buf++;
        buf_size--;
    }

    return char_cnt;
}