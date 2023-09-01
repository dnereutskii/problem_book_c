#ifndef FSTD_H
#define FSTD_H

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
