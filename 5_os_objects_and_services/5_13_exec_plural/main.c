/*
 * Task 5.13
 * Fires external programs up with arguments.
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

#define ARGS_CNT_MIN   2
#define ARGS_DELIMITER ";;"

struct pid_name {
    pid_t pid;
    char *pname;
};

const char *args_delimiter = ARGS_DELIMITER;

static char ** get_delim(char *argv[]);
static int get_delim_indx(char *argv[]);
static size_t get_cnt_prgs(char *argv[]);

int main(int argc, char *argv[])
{
    //int pid, status;

    if (argc < 2) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }
    size_t prg_cnt = get_cnt_prgs(argv);
    /* printf("cnt prgs: %lu\n", prg_cnt); */
    //pid_tbl = melloc(prg_cnt * sizeof(struct pid_name)); 
    char **sub_argv = argv + 1;
    for (size_t i = 0; i < prg_cnt; i++) {
        char **del = get_delim(sub_argv);
        char *del_bck = NULL;
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
        if (del != NULL) {
            *del = del_bck;
            sub_argv = del + 1;
        }
    }
    
    for (size_t i = 0; i < prg_cnt; i++) {
        int status;
        pid_t pid = wait(&status); /* wait for finishing of child process */
        if (WIFEXITED(status) && (WEXITSTATUS(status) == 0)) {
            printf("pid %d\n", pid);
        }
    }
    
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

static int get_delim_indx(char *argv[])
{
    assert(argv != NULL);
    char ** del = get_delim(argv);
    if (del != NULL)
        return (argv - del);
    else
        return (-1);
}

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
