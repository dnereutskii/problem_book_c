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
/* static size_t get_cnt_prgs(char *argv[]); */
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
    pid_t pid;
    int status;
    char delimitered = 1; /* delimiter flag */
    char **sub_argv = argv + 1;
    /* fire programs up */
    while ((*sub_argv != NULL) && delimitered) {
        char **del = get_delim(sub_argv);
        char *del_bck = NULL;
        val_t pn;
        if (del != NULL) {
            del_bck = *del;
            *del = NULL;
        } else {
            delimitered = 0;
        }
        fflush(stderr);
        pid = fork();
        if (pid == -1) { /* fork process error */
            perror("fork failure\n");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) { /* child routine */
            execvp(sub_argv[0], sub_argv);
            perror(sub_argv[0]); /* exec returns control -> error */
            fflush(stderr);
            _exit(1);
        }
        pn.pid = pid;
        pn.pname = sub_argv[0];
        list_add_to_end(table, &pn);
        if (del_bck != NULL) { /* restore delimiter */
            *del = del_bck;
            sub_argv = del + 1;
        }
    }
    /* wait programs */
    while ((pid = wait(&status)) > 0)
    {
        struct node *n = list_search(table, &pid_cmp, (void *)&pid);
        if (n == NULL) {
            fprintf(stderr, "search error\n");
            list_free(table);
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status) && (WEXITSTATUS(status) == 0))
            printf("[pid: %d name: %s]\n", pid, n->val.pname);
        /* if (WIFSIGNALED(status))
            printf("[pid: %d name: %s signaled: %d]\n",
                   pid, n->val.pname, WTERMSIG(status)); */
    };
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
#endif /* not used */

#if 0 /* not used */
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
#endif /* not used */

static bool pid_cmp(struct node *n, void *data)
{
    pid_t *pid = (pid_t *)data;
    if (n->val.pid == *pid)
        return true;
    return false;
}

