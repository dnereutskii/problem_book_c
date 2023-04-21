/*
 * Task 5.01
 * 
 *
 */

#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char **argv)
{
    int f1; /* file descriptor 1 */
    int res; 
    long long cp; /* current position in a file */
    
    if (argc < 2)
	return 1;
    f1 = open(argv[1], O_RDONLY);
    if (f1 == -1)
    {
	fprintf(stderr, "open failed, errno = %d\n", errno);
	return 1;
    }
    cp = lseek64(f1, 0, SEEK_END);    
    printf("%lld\n", cp);
    res = close(f1);
    if (res == -1)
    {
	fprintf(stderr, "close failed, errno = %d\n", errno);
	return 1;
    }
    return 0;
}
