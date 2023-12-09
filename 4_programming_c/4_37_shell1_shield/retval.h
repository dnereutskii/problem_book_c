#ifndef RETVAL_H
#define RETVAL_H

#define ERROR_ALLOCATION "error allocation"

typedef enum retval {
    retval_ok = 0,
    /* Warnings */
    /* Errors */
    retval_err_alloc,
    retval_err,
} retval_t;

#endif /* RETVAL_H */
