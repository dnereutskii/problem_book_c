#ifndef LIST_H
#define LIST_H

typedef struct pid_name {
    pid_t pid;
    char *pname;
} val_t;

struct node {
    val_t val;
    struct node *next;
};

struct list {
    size_t cnt;
    struct node *first;
    struct node *last;
};

struct list * list_init(void);
struct node * list_add_to_end(struct list *l, val_t *val);
struct node * list_add_to_start(struct list *l, val_t *val);
void list_go_through(struct list *l, void (*func)(struct node *, void *), void *data);
void list_free(struct list *l);
void list_delete_last(struct list *l);
struct node * list_search(struct list *l, bool (*cmp)(struct node *, void *),
                          void *data);
#endif /* LIST_H */

