#ifndef FSTD_H
#define FSTD_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>

/*******************************************************************************
 * Macroses
 ******************************************************************************/
/**
 * @brief Swaps two number.
 *
 * @param a Number pointer 1.
 * @param b Number pointer 2.
 * @param type Type of numbers.
 */
#define FSTD_SWAP(a, b, type) \
    do { \
        type tmp = *(type *)b; \
        *(type *)b = *(type *)a; \
        *(type *)a = tmp; \
    } while (0)

/**
 * @brief Returns number from specified range.
 *
 * @param from Low number.
 * @param to High number.
 */
#define FSTD_RAND_RANGE(from, to) \
    (from + (int)((float)to * rand() / (RAND_MAX + 1.0)))


/*******************************************************************************
 * Global function declarations
 ******************************************************************************/
/**
 * @brief 
 * 
 * @param array 
 * @param len 
 * @param ch 
 */
void fstd_fill_array(unsigned char *array, size_t len, unsigned char ch);

/**
 * @brief 
 * 
 * @param array 
 * @param len 
 * @param ch 
 */
void fstd_fill_arr_int(int *arr, size_t len, int num);

/**
 * @brief
 *
 * @param fd File descriptor of opened file.
 * @param filename File name pointer.
 * @param buff Buffer pointer.
 * @param buff_size Buffer size.
 *
 * @return Count of written bytes after success.
 *         -1 after write failure.
 */
int fstd_file_write(int fd, const char *filename,
                    const char *buff, size_t buff_size);

/**
 * @brief
 *
 * @param fd File descriptor of opened file.
 * @param filename File name pointer.
 * @param buff Buffer pointer.
 * @param buff_size Buffer size.
 *
 * @return Count of read bytes after success.
 *         -1 after write failure.
 */
int fstd_file_read(int fd, const char *filename, char *buff, size_t buff_size);

#endif /* FUNCSTD_H */

