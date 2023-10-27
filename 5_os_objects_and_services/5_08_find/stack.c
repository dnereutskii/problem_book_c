#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "stack.h"

static enum stack_report stack_resize(struct stack *stack, size_t new_size);
static void copy(stack_val_t *dest, stack_val_t *src, size_t len);

struct stack * stack_init(size_t size)
{
    struct stack *tmp = malloc(sizeof(struct stack));
    if (tmp == NULL)
        return NULL;
    tmp->items = malloc(size * sizeof(stack_val_t));
    if (tmp->items == NULL) {
        free(tmp);
        return NULL;
    } else {
        tmp->maxsize = size;
        tmp->top = tmp->items;
    }
    return tmp;
}

void stack_destroy(struct stack *stk)
{
    free(stk->items);
    free(stk);
}

enum stack_report stack_push(struct stack *stk, stack_val_t val)
{
    if (stk->top >= stk->items + stk->maxsize)
    {
        enum stack_report rep = stack_resize(stk,
                                             STACK_SIZE_MUL * stk->maxsize);
        if (rep != stack_report_ok)
            return rep;
    }
    *stk->top = val;
    stk->top += 1;
    return stack_report_ok;
}

enum stack_report stack_pop(struct stack *stk, stack_val_t *val)
{
    if (stk->top == stk->items) {/* empty stack */
        return stack_report_underflow;
    } else {
        stk->top -= 1;
        if (val != NULL)
            *val = *stk->top;
        return stack_report_ok;
    }
}

bool stack_is_empty(struct stack *stk)
{
    return (stk->top == stk->items) ? true : false;
}

void stack_print(struct stack *stk)
{
    stack_val_t *tmp = stk->top;
    
    if (tmp == stk->items)
        return; /* empty stack */
    while (tmp != stk->items)
    {
        printf("%d\n", *tmp);
        tmp--;
    }
    printf("%d\n", *tmp);
}

size_t stack_push_array(struct stack *stk, const stack_val_t *array, size_t size)
{
    size_t i = 0;
    while (size != 0) {
        if (stack_push(stk, array[i]) != stack_report_ok)
            break;
        i++;
        size--;
    }
    return i;
}

size_t stack_pop_array(struct stack *stk, stack_val_t *array, size_t size)
{
    size_t i = 0;
    while (size != 0) {
        if (stack_pop(stk, &array[i]) != stack_report_ok)
            break;
        i++;
        size--;
    }
    return i;
}

void stack_pop_until(struct stack *stk, stack_val_t val)
{
    stack_val_t tmp;
    do {
        if (stack_pop(stk, &tmp) != stack_report_ok)
            break;
    } while (tmp != val);
}

static enum stack_report stack_resize(struct stack *stk, size_t new_size)
{
    if (stk->maxsize >= new_size)
        return stack_report_small_new_size;
    stack_val_t *tmp = malloc(new_size * sizeof(stack_val_t));
    if (tmp == NULL)
        return stack_report_malloc_fail;
    copy(tmp, stk->items, stk->maxsize);
    size_t offset = stk->top - stk->items;
    stk->top = tmp + offset;
    free(stk->items);        /* free earlier allocated memory */
    stk->items = tmp;        /* new buffer */
    stk->maxsize = new_size; /* new size */
    return stack_report_ok;
}

static void copy(stack_val_t *dest, stack_val_t *src, size_t len)
{
    assert(dest != NULL);
    assert(src != NULL);
    
    while (len)
    {
        *dest = *src;
        dest++;
        src++;
        len--;
    }
}
