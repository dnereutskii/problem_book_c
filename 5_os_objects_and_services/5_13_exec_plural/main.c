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
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>

#define ARGS_CNT_MIN   2
#define ARGS_DELIMITER ";;"

static char ** get_delimiter(char *argv[]);

int main(int argc, char *argv[])
{
    //int pid, status;

    if (argc < 2) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }
    for (size_t i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    unsigned cnt = 0;
    char **del = argv;
    while ((del = get_delimiter(del)) != NULL)
        cnt++;
    printf("cnt delimiter: %u\n", cnt);
#if 0    
    fflush(stderr);
    pid = fork();
    if (pid == -1) { /* make process error */
        perror("fork failure\n");
        exit(EXIT_FAILURE);
    }
    /* child process */
    if (pid == 0) { 
        //execlp("ls", "ls", "-la", "/var", NULL);
        execvp(argv[1], &argv[1]);
        perror(argv[1]); /* exec returns control -> error */
        fflush(stderr);
        _exit(1);
    }
    /* parent process */
    wait(&status); /* wait for finishing of child process */
    if (WIFEXITED(status))
        printf("exited %d\n", WEXITSTATUS(status));
    else
        printf("killed %d\n", WTERMSIG(status));
#endif   
    return 0;
}

static char ** get_delimiter(char *argv[])
{
    assert(argv != NULL);
    while (*argv != NULL) {
        if (strcmp(*argv, ARGS_DELIMITER) == 0)
            return argv;
        argv++;
    }
    return NULL;
}
