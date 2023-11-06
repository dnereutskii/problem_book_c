/*
 * Task 5.13 exec plural.
 * Fires external programs up with arguments and prints its name when 
 * they finished with zero exit. 
 * Run: ./execplural prg1 <arg1> <arg2> ';;' prg2 <arg1> ';;' prg3 
 *
 * @author rhetti
 * @date 11.2023
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "list.h"

#define ARGS_CNT_MIN   2
#define ARGS_DELIMITER ";;"

const char *args_delimiter = ARGS_DELIMITER;

static char ** get_delim(char *argv[]);
/* static int get_delim_indx(char *argv[]); */
static size_t get_cnt_prgs(char *argv[]);
static bool pid_cmp(struct node *n, void *data);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "too few arguments\n");
        exit(EXIT_FAILURE);
    }
    struct list * table = list_init(); /* pid name table */
    if (table == NULL) {
        fprintf(stderr, "table allocation error\n");
        exit(EXIT_FAILURE);
    }
    size_t prg_cnt = get_cnt_prgs(argv); /* programs to execute */
    char **sub_argv = argv + 1;
    /* fire program up */
    for (size_t i = 0; i < prg_cnt; i++) {
        char **del = get_delim(sub_argv);
        char *del_bck;
        val_t pn;
        if (del != NULL) {
            del_bck = *del;
            *del = NULL;
        }
        fflush(stderr);
        int pid = fork();
        if (pid == -1) { /* fork process error */
            perror("fork failure\n");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) { /* child process */
            execvp(sub_argv[0], sub_argv);
            perror(argv[1]); /* exec returns control -> error */
            fflush(stderr);
            _exit(1);
        }
        pn.pid = pid;
        pn.pname = sub_argv[0];
        list_add_to_end(table, &pn);
        if (del != NULL) {
            *del = del_bck;
            sub_argv = del + 1;
        }
    }
    /* wait programs */
    for (size_t i = 0; i < prg_cnt; i++) {
        int status;
        pid_t pid = wait(&status); /* wait for finishing of child process */
        struct node *n = list_search(table, &pid_cmp, (void *)&pid);
        if (n == NULL) {
            fprintf(stderr, "search error\n");
            list_free(table);
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status) && (WEXITSTATUS(status) == 0)) {
            printf("[pid %d name %s]\n", pid, n->val.pname);
        }
    }
    list_free(table);
    
    return 0;
}

static char ** get_delim(char *argv[])
{
    assert(argv != NULL);
    while (*argv != NULL) {
        if (strcmp(*argv, args_delimiter) == 0)
            return argv;
        argv++;
    }
    return NULL;
}

#if 0 /* not used */
static int get_delim_indx(char *argv[])
{
    assert(argv != NULL);
    char ** del = get_delim(argv);
    if (del != NULL)
        return (argv - del);
    else
        return (-1);
}
#endif

static size_t get_cnt_prgs(char *argv[])
{
    unsigned cnt = 0;
    char **arg = argv + 1;
    if (*arg == NULL)
        return cnt;
    while (*arg != NULL) {
        if (strcmp(*arg, args_delimiter) == 0)
            cnt++;
        arg++;
    }
    if (strcmp(*(arg - 1), args_delimiter) != 0)
        cnt++;
    return cnt;
}

static bool pid_cmp(struct node *n, void *data)
{
    pid_t *pid = (pid_t *)data;
    if (n->val.pid == *pid)
        return true;
    return false;
}

