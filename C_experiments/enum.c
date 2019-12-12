#include <stdio.h>

enum numbers
{
    zero,
    one,
    two,
    fortytwo = 42
};

int main()
{
    enum numbers n;
    n = 44;
    printf("%d %d %d %d %d\n", zero, one, two, fortytwo, n);
}