/*
 * Task 5.10
 * Test fork.
 */

#include <stdio.h>
#include <unistd.h>

int main()
{
    int pid;
    
    printf("start\n");
    pid = fork();
    if (pid == 0)
        printf("child\n");
    else
        printf("parent\n");
    printf("finish\n");
        
    return 0;
}
