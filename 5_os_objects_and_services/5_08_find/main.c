/*
 * @author rhetti
 * @date 09.2023
 *
 * Task 5.08
 * Finds file by opendir, readdir, closedir std functions.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include "stack.h"

#define FILE_NAME argv[1]

static void add_to_path(struct stack *path, const char *name);
static void rm_from_path(struct stack *path);
static void find_file(struct stack *path, char *filename);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }
    char path_str[] = ".";
    struct stack *path = stack_init(20);
    stack_push_array(path, path_str, sizeof(path_str));
    find_file(path, FILE_NAME);
    stack_destroy(path);
    
    return 0;
}

static void find_file(struct stack *path, char *filename)
{
    DIR *dir;
    struct dirent *dent;

    dir = opendir(path->items);
    if (dir == NULL) {
        perror(path->items);
        return;
    }
    while ((dent = readdir(dir)) != NULL) {
        if (dent->d_type == DT_DIR) {
            if ((strcmp(dent->d_name, ".") == 0) ||
                (strcmp(dent->d_name, "..") == 0))
                continue;
            add_to_path(path, dent->d_name);
            if (strcmp(dent->d_name, filename) == 0)
                printf("%s\n", path->items);
            find_file(path, filename);
        } else {
            if (strcmp(dent->d_name, filename) == 0) {
                add_to_path(path, dent->d_name);
                printf("%s\n", path->items);
                rm_from_path(path);
            }
        }
    }
    rm_from_path(path);
    closedir(dir);
}

/**
 * Adds name to path end.
 *
 * @param path Directory path.
 * @param name String.
 */
static void add_to_path(struct stack *path, const char *name)
{
    stack_pop(path, NULL); /* pop null char */
    stack_push(path, '/');
    stack_push_array(path, name, strlen(name));
    stack_push(path, '\0');
}

/**
 * Removes last name from path end.
 *
 * @param path Directory path.
 */
static void rm_from_path(struct stack *path)
{
    stack_pop_until(path, '/');
    stack_push(path, '\0');
}
