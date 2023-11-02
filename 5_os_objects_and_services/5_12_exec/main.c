#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pid, status;

    if (argc < 2) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }
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
        
    return 0;
}
