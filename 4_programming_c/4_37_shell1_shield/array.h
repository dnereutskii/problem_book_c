#ifndef ARRAY_H
#define ARRAY_H

/* Includes ------------------------------------------------------------------*/
#include "retval.h"

/* Defines -------------------------------------------------------------------*/
#define REALLOC_SIZE_FACTOR 2

/* Struct declaration --------------------------------------------------------*/
struct array {
    char *buf;
    size_t limit;
    size_t count;
};

/* Function prototypes -------------------------------------------------------*/
struct array *array_init(size_t len);
retval_t array_save_char(struct array *arr, char ch);
void array_destroy(struct array *arr);
void array_reset(struct array *arr);

#ifndef /* ARRARY_H */
