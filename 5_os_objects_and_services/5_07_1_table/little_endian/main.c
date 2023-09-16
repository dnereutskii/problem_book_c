/*
 * Little-endian test.
 *
 * @author rhetti
 * @date 09.2023
 *
 */
#include <stdio.h>

int main()
{
    unsigned num = 0x11223344;
    char *p = (char *)&num;
    for (size_t i = 0; i < sizeof(num); i++) {
        printf("%x ", p[i]);
    }
    printf("\n");
    
    return 0;
}
