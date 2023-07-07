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
// #include <sys/types.h>
// #include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
// #include <errno.h>
#include <stdlib.h> /* for exit() */
#include <stdbool.h>
#include "strstd.h"


#define MIN_NUM_ARGS    3
#define SIZE_BYTE_STD   4

enum {
    KEY_SIZE,
    KEY_NAME,

    KEY_NUM
};

struct arguments {
    int size_byte;
    const char * file_name;
};

/**
 * @brief Get the values of arguments  
 * 
 * @param argc 
 * @param argv 
 * @param args struct arguments pointer.
 */
static void get_args(int argc, char **argv, struct arguments *args);

const char keys[][5] = {
    "-s",
    "-n",
};

const char *file_name_std = "binary";

int main(int argc, char **argv)
{  
    struct arguments myargs;
    int res;
    unsigned num;

    /* Handling argument keys */
    get_args(argc, argv, &myargs);
    if (!myargs.file_name)
        myargs.file_name = file_name_std;
    if (myargs.size_byte == -1 || myargs.size_byte > SIZE_BYTE_STD)
        myargs.size_byte = SIZE_BYTE_STD;

    /* Create (truncate) binary file */
    int file_bin = open(myargs.file_name, O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (file_bin == -1) {
        perror(myargs.file_name);
        exit(1);
    }

    /* Handling stdin stream */
    while ((res = scanf("%u", &num)) != EOF) {
        // printf("res %d ", res);
        if (res != 1) {  /* there is no converted number */
            int c;
            while ((c = getchar()) != ' ' && c != EOF) {} /* go to space char */
            if (c == EOF)
                break;
            continue;
        }
        // printf("num %d\n", num);
        int wr = 0;
        while (wr != myargs.size_byte) {
            wr += write(file_bin, &num, myargs.size_byte);
        }
    } 

    close(file_bin);

    return 0;
}

static void get_args(int argc, char **argv, struct arguments *args)
{
    args->file_name = NULL;
    args->size_byte = -1;

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
                    res_hand = sscanf(argv[j+1], "%u", &args->size_byte);
                    if (res_hand != 1)
                        args->size_byte = -1;
                    break;
                }
                break;
            }
        }
    }
}
