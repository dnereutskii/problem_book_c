/*
 * Task 5.03.1:
 *
 * Note: For text files use high level input-output, for binary files
 *       use syscalls.
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

#define BLOCK_SIZE 1024

static void direct_flow(void);
static void direct_file(const char *file_name);

int main(int argc, char **argv)
{  
    if (argc < 2)
	direct_flow();
    else
	direct_file(argv[1]);
    

    return 0;
}

void direct_flow(void)
{
    int rc, wc;
    char buf[BLOCK_SIZE];
    char *tmp;
    
    while((rc = read(STDIN_FILENO, buf, BLOCK_SIZE)) > 0) {
	tmp = buf;
	wc = 0;
	while(rc != 0) {
	    tmp += wc;
	    wc = write(STDOUT_FILENO, tmp, rc);
	    rc -= wc;
	}
    }
}

void direct_file(const char *file_name)
{
    int f1; /* file descriptor 1 */
    int res; /* result of operation */
    long long size; /* size of file */
    char mbuf[BLOCK_SIZE];

    f1 = open(file_name, O_RDONLY);
    if (f1 == -1) {
	fprintf(stderr, "open failed, errno = %d\n", errno);
	return;
    }
    size = lseek64(f1, 0, SEEK_END);    
    for (size_t i = 0; i < (size / BLOCK_SIZE); i++) {
	lseek(f1, i * BLOCK_SIZE, SEEK_SET);
	res = read(f1, mbuf, BLOCK_SIZE);
	if (res == -1) {
	    fprintf(stderr, "read failed, errno = %d\n", errno);
	    return;
	}
	res = write(1, mbuf, BLOCK_SIZE);
	if (res == -1) {
	    fprintf(stderr, "write failed, errno = %d\n", errno);
	    return;
	}
    }
    lseek(f1, -(size % BLOCK_SIZE), SEEK_END);
    res = read(f1, mbuf, size % BLOCK_SIZE);
    if (res == -1) {
	fprintf(stderr, "read failed, errno = %d\n", errno);
	return;
    }
    res = write(1, mbuf, size % BLOCK_SIZE);
    if (res == -1) {
	fprintf(stderr, "write failed, errno = %d\n", errno);
	return;
    }
    res = close(f1);
    if (res == -1) {
	fprintf(stderr, "close failed, errno = %d\n", errno);
	return;
    }
}
