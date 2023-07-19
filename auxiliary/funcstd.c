#include <stdio.h>
#include "funcstd.h"

void fill_array(unsigned char *array, size_t len, unsigned char ch)
{
    while (len) {
        *array = ch;
        array++;
        len--;
    }
}