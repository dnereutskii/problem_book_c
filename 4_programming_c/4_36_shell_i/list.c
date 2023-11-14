#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "list.h"

static void free_node(struct node *n, void *data);
static void save_val(struct node *n, val_t val);
/*static void print_node(struct node *n, void *data);*/

struct list * list_init(void)
{
    struct list *new_list = malloc(sizeof(struct list));
    if (new_list != NULL) {
        new_list->first = NULL;
        new_list->last = NULL;
    } else {
        perror("list_init");
    }
    return new_list;
}

struct node * list_add_to_end(struct list *l, val_t val)
{
    assert(l != NULL);   
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL)
        return NULL;
    save_val(new_node, val);
    new_node->next = NULL;
    if (l->last == NULL) {
        l->last = new_node;
        l->first = new_node;
    } else {
        l->last->next = new_node;
        l->last = new_node;
    }
    return new_node;
}

struct node * list_add_to_start(struct list *l, val_t val)
{
    assert(l != NULL);
    
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL)
        return NULL;
    save_val(new_node, val);
    if (l->first == NULL) {
        new_node->next = NULL;
        l->first = new_node;
        l->last = new_node;
    } else {
        new_node->next = l->first;
        l->first = new_node;
    }
    return new_node;
}

void list_delete_last(struct list *l)
{
    assert(l != NULL);
    if (l->last == l->first) { /* one node */
        free_node(l->last, NULL);
        l->last = NULL;
        l->first = NULL;
        return;
    }
    struct node *tmp = l->first;
    while (tmp->next != l->last)
        tmp = tmp->next;
    free_node(l->last, NULL);
    tmp->next = NULL;
    l->last = tmp;
}

void list_go_through(struct list *l, void (*func)(struct node *, void *), void *data)
{
    assert(l != NULL);
    assert(func != NULL);

    struct node *tmp = l->first;
    if (tmp == NULL) /* empty list */
        return;
    while (tmp != NULL) {
        struct node *next = tmp->next;
        func(tmp, data);
        tmp = next;
    }    
}

struct node * list_search(struct list *l, bool (*cmp)(struct node *, void *),
                          void *data)
{
    assert(l != NULL);
    assert(cmp != NULL);

    struct node *node = l->first;
    if (node == NULL) /* empty list */
        return NULL;
    while (node != NULL) {
        bool find = cmp(node, data);
        if (find)
            return node;
        node = node->next;
    }
    return NULL;
}

void list_destroy(struct list *l)
{
    assert(l != NULL);

    list_go_through(l, &free_node, NULL);
    free(l);
}

void list_free(struct list *l)
{
    assert(l != NULL);

    list_go_through(l, &free_node, NULL);
    l->first = NULL;
    l->last = NULL;
    l->cnt = 0;
}  
 
static void free_node(struct node *n, void *data)
{
    (void) data;
    assert(n != NULL);
    free(n);
}

static void save_val(struct node *n, val_t val)
{
    n->val = val;
}

