## Problem

When `scanf` doesn't work, the invalid data is still left in the stream. You'll have to read and discard the data from the stream first before you can enter more data.

```
#include<stdio.h>

int main(void) {
    int val,x;
    x=scanf("%d",&val);
    if(x==1)
        printf("success!");
    else{
        // Discard everything upto and including the newline.
        while ( (x = getchar()) != EOF && x != '\n' );
        printf("try again\n");
        scanf("%d",&val);
    }
    return 0;
}
```

The scanf family of functions are broken-as-specified and should never be used for anything.
The correct way to write this program is to use `getline`, if available, or `fgets` otherwise, to read an entire line of user input. Then use `strtol` to convert the input to a machine integer, taking care to check for errors:

```
errno = 0;
result = strtol(line, &endptr, 10);
if (endptr == line || *endptr != '\n' || errno)
   // invalid input
```

## Sources
1. https://stackoverflow.com/a/34552538/7758330