/*
 * @author rhetti
 * @date 10.2023
 *
 * Task 5.09
 * Print stat and lstat info for a file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define PATH_NAME argv[1]

static void print_dev(const struct stat *s);
static void print_ino(const struct stat *s);
static void print_mode(const struct stat *s);
static void print_type(const struct stat *s);
static void print_nlink(const struct stat *s);
static void print_uid(const struct stat *s);
static void print_gid(const struct stat *s);
static void print_rdev(const struct stat *s);
static void print_size(const struct stat *s);
static void print_blksize(const struct stat *s);
static void print_blocks(const struct stat *s);
static void print_atim(const struct stat *s);
static void print_mtim(const struct stat *s);
static void print_ctim(const struct stat *s);
static void print_stat(const struct stat *s);

int main(int argc, char **argv)
{
    struct stat sinfo;
    
    if (argc < 2) {
        fprintf(stderr, "too few arguments\n");
        exit(EXIT_FAILURE);
    }
    if (lstat(PATH_NAME, &sinfo) == -1) {
        perror(PATH_NAME);
        exit(EXIT_FAILURE);
    }
    print_stat(&sinfo);
    if (S_ISLNK(sinfo.st_mode)) {
        printf("\n");
        if (stat(PATH_NAME, &sinfo) == -1) {
            if (errno == ENOENT) {
                printf("dangling\n");
            } else {
                perror(PATH_NAME);
                exit(EXIT_FAILURE);
            }
        } else {
            print_stat(&sinfo);
        }
    }
    
    return 0;
}

static void print_dev(const struct stat *s)
{
    assert(s != NULL);
    printf("ID of device containig file (maj, min): %#X %#X \n", major(s->st_dev),
           minor(s->st_dev));
}

static void print_ino(const struct stat *s)
{
    assert(s != NULL);
    printf("I-node: %lu\n", s->st_ino);
}

static void print_type(const struct stat *s)
{
    assert(s != NULL);
    printf("File type: ");
    if (S_ISREG(s->st_mode))
        printf("regular file\n");
    else if (S_ISDIR(s->st_mode))
        printf("directory\n");
    else if (S_ISCHR(s->st_mode))
        printf("character device\n");
    else if (S_ISBLK(s->st_mode))
        printf("block device\n");
    else if (S_ISFIFO(s->st_mode))
        printf("FIFO\n");
    else if (S_ISLNK(s->st_mode))
        printf("symbolik link\n");
    else if (S_ISSOCK(s->st_mode))
        printf("socket\n");
}

static void print_mode(const struct stat *s)
{
    assert(s != NULL);
    printf("File mode: %#o\n", s->st_mode & (~S_IFMT));
}

static void print_size(const struct stat *s)
{
    assert(s != NULL);
    printf("Total size: %ld\n", s->st_size);
}

static void print_nlink(const struct stat *s)
{
    assert(s != NULL);
    printf("Hard link number: %ld\n", s->st_nlink);
}

static void print_uid(const struct stat *s)
{
    assert(s != NULL);
    printf("User ID of  owner: %d\n", s->st_uid);
}

static void print_gid(const struct stat *s)
{
    assert(s != NULL);
    printf("Group ID of owner: %d\n", s->st_gid);
}

static void print_rdev(const struct stat *s)
{
    assert(s != NULL);
    if (S_ISCHR(s->st_mode) || S_ISBLK(s->st_mode))
        printf("Device ID (maj, min): %#X %#X\n", major(s->st_rdev),
               minor(s->st_rdev));
}

static void print_blksize(const struct stat *s)
{
    assert(s != NULL);
    printf("Block size for filesystem I/O: %ld\n", s->st_blksize);
}

static void print_blocks(const struct stat *s)
{
    assert(s != NULL);
    printf("Number of 512 B blocks allocated: %ld\n", s->st_blocks);
}

static void print_atim(const struct stat *s)
{
    printf("Last file access: %s", ctime(&s->st_atim.tv_sec));
}

static void print_mtim(const struct stat *s)
{
    printf("Last file modification: %s", ctime(&s->st_mtim.tv_sec));
}

static void print_ctim(const struct stat *s)
{
    printf("Last status changed: %s", ctime(&s->st_ctim.tv_sec));
}

static void print_stat(const struct stat *s)
{
    print_type(s);
    print_mode(s);
    print_dev(s);
    print_ino(s);
    print_size(s);
    print_nlink(s);
    print_uid(s);
    print_gid(s);
    print_rdev(s);
    print_blksize(s);
    print_blocks(s);
    print_ctim(s);
    print_atim(s);
    print_mtim(s);    
}
