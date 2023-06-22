#include <stdio.h>
#include "strstd.h"

bool string_compare(const char *str1, const char *str2)
{
    bool res = true;

    while(*str1 && *str2) {
        if (*str1 != *str2) {
            res = false;
            break;
        }
        str1++;
        str2++;
    };
    if (*str1 != *str2)
        res = false;

    return res;
}

size_t string_length(const char *str)
{
    size_t cnt = 0;

    while (*str) {
        cnt++;
        str++;
    }

    return cnt;
}