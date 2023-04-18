#include <stdio.h>
#include <stdlib.h>
#include "header.h"
void counting_Sempahore_init(counting_Semaphore *cou,int value)
{
    pthread_mutex_init(&cou->mutex,NULL);
    pthread_cond_init(&cou->cond,NULL);
    cou->value = value;
}

void counting_Semaphore_Wait(counting_Semaphore *cou)
{
    pthread_mutex_lock(&cou->mutex);
    //if(buffer->produceComplete)return;
    //printf("\nI am con from %d and struck waiting here\n",from);
    while (cou->value <= 0)
    {
        pthread_cond_wait(&cou->cond,&cou->mutex);
    }
    cou->value--;
    pthread_mutex_unlock(&cou->mutex);
}

void counting_Semaphore_Cond(counting_Semaphore *cou)
{
    pthread_mutex_lock(&cou->mutex);
    cou->value++;
    pthread_cond_signal(&cou->cond);
    pthread_mutex_unlock(&cou->mutex);
}
