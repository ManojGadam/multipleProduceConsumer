#include <stdio.h>
#include "sum.h"
int sum(int *A,int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum+=A[i];
    }
    return sum;
}