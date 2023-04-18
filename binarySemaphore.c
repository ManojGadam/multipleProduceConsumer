#include <stdio.h>
#include <stdlib.h>
#include "header.h"
void binary_Sempahore_init(binary_Semaphore *bin,int value)
{
    pthread_mutex_init(&bin->mutex,NULL);
    pthread_cond_init(&bin->cond,NULL);
    bin->value = value;
}

void binary_Sempahore_Wait(binary_Semaphore *bin)
{
    pthread_mutex_lock(&bin->mutex);
    //printf("\nI am bin and struck waiting here\n");
    while (bin->value <= 0)
    {
        pthread_cond_wait(&bin->cond,&bin->mutex);
    }
    bin->value=0;
    pthread_mutex_unlock(&bin->mutex);
}

void binary_Semaphore_Cond(binary_Semaphore *bin)
{
    pthread_mutex_lock(&bin->mutex);
    bin->value=1;
    pthread_cond_signal(&bin->cond);
    pthread_mutex_unlock(&bin->mutex);
}