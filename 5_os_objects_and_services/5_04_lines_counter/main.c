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

#define READ_BLOCK_SIZE 4096
#define MIN_ARGS_CNT    2

int main(int argc, char **argv)
{
    if (argc < MIN_ARGS_CNT) {
	fprintf(stderr, "too few arguments\n");
	return 1;
    }

    char *text_file_name = argv[1];
    int text_file = open(text_file_name, O_RDONLY|O_APPEND);
    if (text_file == 0) {
	perror(text_file_name);
	return 1;
    }

    close(text_file);
    
    return 0;
}
