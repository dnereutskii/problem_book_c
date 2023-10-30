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
    pid = fork();
    if (pid == -1) { /* make process error */
        perror("fork failure\n");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) { /* child process */   
        //execlp("ls", "ls", "-la", "/var", NULL);
        if (execvp(argv[1], &argv[1]) == -1) {
                perror(argv[1]); /* exec returns control -> error */
                exit(EXIT_FAILURE);
        }
        //exit(EXIT_SUCCESS);
    }
    /* parent process */
    wait(&status); /* wait for finishing of child process */
    printf("%d\n", status);
        
    return 0;
}
