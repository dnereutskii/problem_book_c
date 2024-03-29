#include <stdio.h>
#include <assert.h>
#include "strstd.h"

#define INT_STR_MAX_LEN (10)

bool strstd_compare(const char *str1, const char *str2)
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

size_t strstd_length(const char *str)
{
    size_t cnt = 0;

    while (*str) {
        cnt++;
        str++;
    }

    return cnt;
}

char * strstd_sub_search(const char *str, const char *sub)
{
    size_t cnt_cmp = 0;
    size_t sub_len = strstd_length(sub);
    const char *sub_cur = sub;
    const char *sub_ret = NULL;

    while (*str) {
        if (*sub_cur == *str) {
            if (!cnt_cmp)
                sub_ret = str;
            cnt_cmp++;
            sub_cur++;
        }
        else {
            if (cnt_cmp) {
                sub_cur = sub;
                cnt_cmp = 0;
                sub_ret = NULL;
            }
        }
        if (*sub_cur == '\0')
            break;
        str++;
    }
    if (cnt_cmp != sub_len)
        sub_ret = NULL;
    
    return (char *)sub_ret;
}

size_t strstd_copy(char *dest, size_t dest_size, const char *src)
{
    size_t cnt_copied = 0;
    
    while ((*src) && (cnt_copied != dest_size)) {
        *dest = *src;
        dest++;
        src++;
        cnt_copied++;
    }
    *dest = '\0';
    
    return (cnt_copied - 1);
}

int strstd_stoi(const char *str)
{
	assert(str != NULL);
    
    size_t cnt = 0;
	int num = 0;
    int sign = 1;

    if (*str == '-') {
        sign = -1;
        str++;
    }
	while ((*str) && (cnt != INT_STR_MAX_LEN)) {
		if (*str > '9' && *str < '0')
			break;
		num = num * 10 + (*str - '0');
		str++;
	}
    
	return num * sign;
}	

