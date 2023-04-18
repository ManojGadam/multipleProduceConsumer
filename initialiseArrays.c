#include <stdio.h>
#include "initialiseArrays.h"
void initialiseArrays(int *A,int size)
{
    for (int i = 0; i < size; i++)
    {
        A[i] = 0;
    }
    
}