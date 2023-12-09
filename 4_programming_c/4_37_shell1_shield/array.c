#include <stdio.h>
#include <strings.h>
#include "array.h"

/* Function definitions ------------------------------------------------------*/
struct array *array_init(size_t len)
{
    struct array *arr = malloc(sizeof(struct array));
    if (arr == NULL) {
        perror(ERROR_ALLOCATION);
        return NULL;
    }
    arr->buf = malloc(len * sizeof(char));
    if (arr->buf == NULL) {
        perror(ERROR_ALLOCATION);
        free(arr);
        return NULL;
    }
    arr->count = 0;
    arr->limit = len;
    return arr;
}

int array_save_char(struct array *arr, char ch)
{
    if (arr->count >= arr->limit) {
        arr->buf = realloc(arr->buf, arr->limit * SIZE_FACTOR);
        if (arr->buf == NULL) {
            perror("array_safe_char()");
            return retval_err_alloc;
        }
        arr->limit *= REALLOC_SIZE_FACTOR;
    }
    arr->buf[arr->count] = ch;
    arr->count++;
    return retval_ok;
}

void array_destroy(struct array *arr)
{
    if (arr->buf != NULL)
        free(arr->buf);
    free(arr);
}

void array_reset(struct array *arr)
{
    arr->count = 0;
    arr->buf[0] = '\0';
}

