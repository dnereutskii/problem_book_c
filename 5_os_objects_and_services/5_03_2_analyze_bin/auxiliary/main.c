/*
 * Task 5.03.2 (ref to 2.54) auxiliary program.
 *
 * Makes binary file with numbers from stdin stream.
 * Run: ./prog -s 1 -n filename << stream of numbers
 * 
 * @note: For text files use high level input-output, for binary files
 *       use syscalls.
 * 
 * @author rhetti
 * @date   06.2023
 * 
 */

#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h> /* for exit() */
#include <string.h>
#include <stdbool.h>
#include "strstd.h"


#define MIN_NUM_ARGS 3

enum {
    KEY_SIZE,
    KEY_NAME,

    KEY_NUM
};

struct arguments {
        int size;
        const char * file_name;
};

static void get_args(int argc, char **argv, struct arguments *args);


const char keys[][5] = {
        "-s",
        "-n",
};

int main(int argc, char **argv)
{  
    // if (argc < 2) {
    //     fprintf(stderr, "too few arguments\n");
    //     return 1;
    // }

    // FILE *file_text_wr = fopen(file_name, "w"); /* truncate or create for writing */
    // if (file_text_wr == NULL) {
    //     perror(argv[2]);
    //     fclose(file_text_rd);
    //     exit(2);
    // }
 
    // int file_bin = open(file_name, O_WRONLY|O_CREAT|O_TRUNC, 0666);
    // if (file_bin == -1) {
    //     perror(argv[3]);
    //     exit(3);
    // }
    struct arguments myargs;
    get_args(argc, argv, &myargs);
    if (myargs.file_name)
        printf("-n = %s\n", myargs.file_name);
    if (myargs.size != -1)
        printf("-s = %u\n", myargs.size);

    return 0;
}

static void get_args(int argc, char **argv, struct arguments *args)
{
    args->file_name = NULL;
    args->size = -1;

    if (argc < MIN_NUM_ARGS)
        return;

    for (size_t i = 0; i < KEY_NUM; i++) {
        
        for (size_t j = 1; j < argc; j++) {
            bool res_cmp = string_compare(argv[j], keys[i]);
            if (res_cmp) {
                int res_hand;
                switch (i)
                {
                case KEY_NAME:
                    args->file_name = argv[j+1];
                    break;
                case KEY_SIZE:
                    res_hand = sscanf(argv[j+1], "%u", &args->size);
                    if (res_hand != 1)
                        args->size = -1;
                    break;
                }
                break;
            }
        }
    }
}