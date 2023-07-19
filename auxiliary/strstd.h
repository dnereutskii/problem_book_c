#ifndef STRSTD_H
#define STRSTD_H

#include <stdbool.h>

/**
 * @brief Non-lexicographically comparing of strings.
 * 
 * @note  Pointers is nul-terminated strings (correct string).
 * 
 * @param str1 String 1 pointer.
 * @param str2 String 2 pointer.
 * @return true  Strings are equel.
 * @return false Strings are not equel.
 */
bool string_compare(const char *str1, const char *str2);

/**
 * @brief 
 * 
 * @param str 
 * @return unsigned int 
 */
size_t string_length(const char *str);

/**
 * @brief Search first entrance for a substring in a string.
 * 
 * @param str String pointer.
 * @param sub Substring pointer.
 * @return char* Start pointer for substring.
 */
char * string_sub_search(const char *str, const char *sub);

#endif /* STRSTD_H */