#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <dirent.h>
#include "strstd.h"
#include "list.h"

static void print_path(struct list *l, char *file_name);
static void print_node1(struct node *n, void *data);
static void find_file(char *dir_name, char *file_name, struct list *l);

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }
    struct list *mylist = list_init();
    find_file(".", argv[1], mylist);
    list_free(mylist);
    
    return 0;
}

static void find_file(char *dir_name, char *file_name, struct list *l)
{
    DIR *dir;
    struct dirent *dent;
    
    dir = opendir(dir_name);
    if (dir == NULL) {
        perror(dir_name);
        return;
    }
    list_add_to_end(l, (val_t)dir_name);
    while((dent = readdir(dir)) != NULL) {
        if (dent->d_type == DT_DIR) {
            if ((strstd_compare(dent->d_name, ".") == true) ||
                (strstd_compare(dent->d_name, "..") == true))
                continue;
            find_file(dent->d_name, file_name, l);
        } else {
            if (strstd_compare(file_name, dent->d_name) == true)
                print_path(l, file_name);
        }
    }
    list_delete_last(l);
    closedir(dir);
}

static void print_path(struct list *l, char *file_name)
{
    char c = '/';
    list_go_through(l, &print_node1, (void *)&c);
    printf("%s\n", file_name);
}

static void print_node1(struct node *n, void *data)
{
    assert(n != NULL);

    printf("%s%c", n->val, *(char *)data);
}

