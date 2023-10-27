#ifndef STACK_H
#define STACK_H

#define STACK_INIT_SIZE  10
#define STACK_SIZE_MUL   2

typedef char stack_val_t;

enum stack_report {
    stack_report_ok = 0,
    stack_report_malloc_fail,
    stack_report_small_new_size,
    stack_report_overflow,
    stack_report_underflow,
};

struct stack {
    stack_val_t *items;  /* Array */
    size_t maxsize;  /* Array size */
    stack_val_t *top;    /* Stack top */
};

struct stack * stack_init(size_t size);
enum stack_report stack_push(struct stack *stk, stack_val_t val);
enum stack_report stack_pop(struct stack *stk, stack_val_t *val);
bool stack_is_empty(struct stack *stk);
void stack_destroy(struct stack *stk);
void stack_print(struct stack *stk);
size_t stack_push_array(struct stack *stk, const stack_val_t *array, size_t size);
size_t stack_pop_array(struct stack *stk, stack_val_t *array, size_t size);
void stack_pop_until(struct stack *stk, stack_val_t val);

#endif /* STACK_H */
 
