#include<stdlib.h>

int main()
{
    int* p = malloc(100);
    p[0] = 1;
    p[1] = 3;
    free(p);
    return 0;
}
